#include "RigidBody.h"
#include "Engine.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

namespace why
{
	RigidBody::RigidBody(BodyType type, const std::shared_ptr<Collider>& collider, float mass, float friction)
		: m_type(type), m_collider(collider), m_mass(mass), m_friction(friction)
	{
		if (!collider)
		{
			return;
		}

		/*
		惯性张量计算：
			Static/Kinematic 惯性保持 0，无转动；
			动态刚体根据碰撞形状 + 质量自动计算惯性，决定旋转难易程度；
			质量为 0 也跳过计算。
		*/
		btVector3 intertia(0, 0, 0);
		if (m_type == BodyType::Dynamic && mass > 0.0f && m_collider->GetShape())
		{
			m_collider->GetShape()->calculateLocalInertia(btScalar(mass), intertia);
		}

		//创建运动状态对象：存储刚体初始位置旋转，Bullet 通过它同步物理世界变换和游戏逻辑变换。初始变换为原点单位矩阵。
		btTransform transform;
		transform.setIdentity();
		btDefaultMotionState* motionState = new btDefaultMotionState(transform);


		/*
		构造刚体信息结构体：
		Dynamic 传入真实 mass，Static/Kinematic 强制 mass=0；
		绑定 motionState、碰撞形状、惯性张量。
		*/
		btRigidBody::btRigidBodyConstructionInfo info(
			(m_type == BodyType::Dynamic) ? btScalar(mass) : btScalar(0),
			motionState, m_collider->GetShape(), intertia
		);

		//分配原生 btRigidBody 对象，智能指针托管自动释放；设置表面摩擦力。
		m_body = std::make_unique<btRigidBody>(info);
		m_body->setFriction(friction);

		/*
		运动学刚体专属配置：
		开启 CF_KINEMATIC_OBJECT 标记，Bullet 规则：不受重力、碰撞冲击力，只能代码驱动位置；
		DISABLE_DEACTIVATION：永不休眠，持续参与碰撞检测（门、电梯持续碰撞物体必备）。
		*/
		if (m_type == BodyType::Kinematic)
		{
			m_body->setCollisionFlags(m_body->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
			m_body->setActivationState(DISABLE_DEACTIVATION);
		}
	}

	RigidBody::~RigidBody()
	{
		if (m_addedToWorld)
		{			
			SINGLETON_PTR(Engine)->GetPhysicsManager().RemoveRigidBody(this);
		}
	}

	btRigidBody* RigidBody::GetBody()
	{
		return m_body.get();
	}

	void RigidBody::SetAddedToWorld(bool added)
	{
		m_addedToWorld = added;
	}

	bool RigidBody::IsAddedToWorld() const
	{
		return m_addedToWorld;
	}

	BodyType RigidBody::GetType() const
	{
		return m_type;
	}

	void RigidBody::SetPosition(const glm::vec3& pos)
	{
		if (!m_body)
		{
			return;
		}

		auto& tr = m_body->getWorldTransform();

		tr.setOrigin(btVector3(btScalar(pos.x), btScalar(pos.y), btScalar(pos.z)));
		//setWorldTransform：刚体自身物理变换；
		//motionState->setWorldTransform：同步运动状态缓存，避免物理步进后位置被覆盖；
		if (m_body->getMotionState())
		{
			m_body->getMotionState()->setWorldTransform(tr);
		}
		m_body->setWorldTransform(tr);
	}

	glm::vec3 RigidBody::GetPosition() const
	{
		const auto& pos = m_body->getWorldTransform().getOrigin();
		return glm::vec3(pos.x(), pos.y(), pos.z());
	}

	void RigidBody::SetRotation(const glm::quat& rot)
	{
		if (!m_body)
		{
			return;
		}

		auto& tr = m_body->getWorldTransform();
		tr.setRotation(btQuaternion(btScalar(rot.x), btScalar(rot.y), btScalar(rot.z), btScalar(rot.w)));
		if (m_body->getMotionState())
		{
			m_body->getMotionState()->setWorldTransform(tr);
		}
		m_body->setWorldTransform(tr);
	}

	glm::quat RigidBody::GetRotation() const
	{
		const auto& rot = m_body->getWorldTransform().getRotation();
		return glm::quat(rot.w(), rot.x(), rot.y(), rot.z());
	}
}