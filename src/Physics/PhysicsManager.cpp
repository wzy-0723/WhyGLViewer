#include "PhysicsManager.h"
#include "RigidBody.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace why
{
    PhysicsManager::PhysicsManager()
    {
    }

    PhysicsManager::~PhysicsManager()
    {
    }

    void PhysicsManager::Init()
    {
        //宽相碰撞检测（粗筛）：快速剔除远距离不可能碰撞的物体，大幅减少精细碰撞计算开销
        m_broadphase = std::make_unique<btDbvtBroadphase>();
        //碰撞全局配置：存储碰撞算法缓存、内存分配策略、支持的碰撞形状、堆栈大小等全局参数，所有碰撞逻辑依赖此配置。
        m_collisionConfig = std::make_unique<btDefaultCollisionConfiguration>();
        //窄相碰撞调度器：对宽相筛选出的潜在碰撞对，执行精准几何体碰撞检测（如盒子、球体、凸包相交判断）。
        m_dispatcher = std::make_unique<btCollisionDispatcher>(m_collisionConfig.get());
        //约束求解器：处理刚体碰撞响应、摩擦力、弹力、关节约束（铰链、弹簧等），计算碰撞后速度、位置修正，是物理运动核心计算单元。
        m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
        //创建离散动力学物理世界，把上面 4 个组件组装成完整物理场景，所有刚体、碰撞、模拟都在这个 world 中管理。
        m_world = std::make_unique<btDiscreteDynamicsWorld>(
            m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfig.get()
        );
        //设置全局重力，Y 轴向下 9.81加速度，符合现实物理，所有动态刚体都会受重力持续下落。
        m_world->setGravity(btVector3(0, -9.81f, 0));
    }

    void PhysicsManager::Update(float deltaTime)
    {
        //固定物理步长，强制物理每一步模拟 1/60 秒，保证不同帧率下物理运动速度一致，解决高 / 低帧率物理快慢不一问题。
        const btScalar fixedTimeStep = 1.0f / 60.0f;
        //最大子步数量。若帧间隔过大（如卡顿 0.1s），最多拆分 4 次固定步长模拟，防止一次性计算过多卡死。
        const int maxSubsteps = 4;
        //根据传入帧时间，自动拆分若干个 1/60s 固定子步，逐次执行碰撞检测、约束求解、更新刚体位置旋转。
        m_world->stepSimulation(deltaTime, maxSubsteps, fixedTimeStep);
    }

    void PhysicsManager::AddRigidBody(RigidBody* body)
    {
        if (!body || !m_world)
        {
            return;
        }

        if (auto rigidBody = body->GetBody())
        {
            m_world->addRigidBody(rigidBody, btBroadphaseProxy::StaticFilter,
                btBroadphaseProxy::AllFilter);
            body->SetAddedToWorld(true);
        }
    }

    void PhysicsManager::RemoveRigidBody(RigidBody* body)
    {
        if (!body || !m_world)
        {
            return;
        }

        if (auto rigidBody = body->GetBody())
        {
            m_world->removeRigidBody(rigidBody);
            body->SetAddedToWorld(false);
        }
    }

    btDiscreteDynamicsWorld* PhysicsManager::GetWorld()
    {
        return m_world.get();
    }
}