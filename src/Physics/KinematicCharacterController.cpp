#include "KinematicCharacterController.h"
#include "Engine.h"

#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

namespace why
{
    KinematicCharacterController::KinematicCharacterController(float raduis, float height)
        : m_radius(raduis), m_height(height)
    {
        
        auto world = SINGLETON_PTR(Engine)->GetPhysicsManager().GetWorld();
        // 创建胶囊碰撞形状 btCapsuleShape
        // 参数1：胶囊两端半圆球体半径；参数2：胶囊中间圆柱部分高度
        auto capsule = new btCapsuleShape(m_radius, m_height);

        // 创建带碰撞对缓存的幽灵对象
        m_ghost = std::make_unique<btPairCachingGhostObject>();
        btTransform start;      
        start.setIdentity();                                    // 初始化变换矩阵为单位矩阵（无位移无旋转）
        start.setOrigin(btVector3(0.0f, 2.0f, 0.0f));           // 设置初始生成世界坐标
        m_ghost->setWorldTransform(start);                      // 给幽灵体赋予初始位姿
        m_ghost->setCollisionShape(capsule);
        //CF_CHARACTER_OBJECT：碰撞标记，告知 Bullet 该物体是玩家角色，物理求解时使用角色专属碰撞规则。
        m_ghost->setCollisionFlags(m_ghost->getCollisionFlags() | btCollisionObject::CF_CHARACTER_OBJECT);

        // 粗检测阶段会筛选所有潜在相交物体对，btGhostPairCallback 是幽灵体专用回调，
        // 用来维护幽灵物体实时重叠碰撞列表，供角色控制器判断地面、墙体碰撞。
        world->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(
            new btGhostPairCallback());

        /*
        btKinematicCharacterController 构造入参：
        绑定的幽灵碰撞体：角色位置、碰撞依附该对象；
        碰撞几何体：胶囊；
        stepHeight：角色能够自动迈上的最大台阶高度。
        */
        const btScalar stepHeight = 0.35f; // how high we can step up
        m_controller = std::make_unique<btKinematicCharacterController>(m_ghost.get(), capsule, stepHeight);

        m_controller->setMaxSlope(btRadians(50.0f));
        m_controller->setGravity(world->getGravity()); // negative value is fine
        /*
        addCollisionObject 三个参数：
        待添加碰撞对象指针；
        碰撞所属分组：CharacterFilter = 角色层级；
        碰撞掩码：规定该物体可以和哪些分组物体发生碰撞；当前配置为与所有实体碰撞，排除传感器触发器。
        */
        world->addCollisionObject(
            m_ghost.get(),
            btBroadphaseProxy::CharacterFilter,
            btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger); // collide with most things
        // btKinematicCharacterController 继承 btActionInterface，属于每帧需要更新的动作对象
        // 加入物理世界后，每次物理步进迭代，世界会自动执行控制器的运动、碰撞、重力计算
        world->addAction(m_controller.get());
    }

    KinematicCharacterController::~KinematicCharacterController()
    {
        auto world = SINGLETON_PTR(Engine)->GetPhysicsManager().GetWorld();
        if (m_controller)
        {
            world->removeAction(m_controller.get());// 将控制器从物理世界每帧更新队列中移除
        }
        if (m_ghost)
        {
            world->removeCollisionObject(m_ghost.get());// 将幽灵碰撞体从物理碰撞空间中移除，不再参与碰撞检测
        }
    }


    glm::vec3 KinematicCharacterController::GetPosition() const
    {
        const auto& pos = m_ghost->getWorldTransform().getOrigin();
        const glm::vec3 offset(0.0f, m_height + 0.5f + m_radius, 0.0f);// getWorldTransform()：获取幽灵体当前世界变换；getOrigin()：取出中心点坐标
        return glm::vec3(pos.x(), pos.y(), pos.z()) + offset; // 将Bullet三维向量转为glm向量，并叠加竖直方向偏移量
    }

    glm::quat KinematicCharacterController::GetRotation() const
    {
        const auto& rot = m_ghost->getWorldTransform().getRotation();
        return glm::quat(rot.w(), rot.x(), rot.y(), rot.z());
    }

    void KinematicCharacterController::Walk(const glm::vec3& direction)
    {
        m_controller->setWalkDirection(btVector3(
            btScalar(direction.x),
            btScalar(direction.y),
            btScalar(direction.z)));
    }

    void KinematicCharacterController::Jump(const glm::vec3& direction)
    {
        if (m_controller->onGround())
        {
            m_controller->jump(btVector3(
                btScalar(direction.x),
                btScalar(direction.y),
                btScalar(direction.z)));
        }
    }

    bool KinematicCharacterController::OnGround() const
    {
        return m_controller->onGround();
    }
}