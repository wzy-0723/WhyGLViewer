#pragma once
#include "Collider.h"

#include <glm/gtc/quaternion.hpp>

#include <memory>

/*
* btRigidBody 内部依赖构成（创建刚体必备四件套）
btCollisionShape：碰撞形状（球体、立方体、胶囊、凸包等），决定碰撞边界；
btDefaultMotionState：运动状态存储，记录物体世界位姿，负责物理世界 ↔ 渲染层坐标同步；
btRigidBodyConstructionInfo：刚体构造信息结构体，汇总质量、摩擦、弹性、阻尼等所有物理参数；
btDynamicsWorld：刚体添加进物理世界后，每帧步进计算受力、碰撞、位置更新。
*/

class btRigidBody;

namespace why
{
	enum class BodyType
	{
		// 地面墙壁等大型不可移动障碍物
		Static,
		// 普通物体，受重力和碰撞的影响
		Dynamic,
		// 运动学刚体，通过代码手动控制，例如移动平台，电梯，玩家胶囊体
		Kinematic
	};
	/*
	对 Bullet btRigidBody 的上层封装类，统一管理静态 / 动态 / 运动学三类刚体，
	绑定碰撞体 Collider，提供 glm 风格位置旋转读写接口，
	自动处理物理世界卸载、生命周期管理，隔离游戏业务与原生 Bullet 底层 API。
	*/
	class RigidBody
	{
	public:
		RigidBody(BodyType type, const std::shared_ptr<Collider>& collider, float mass, float friction);
		~RigidBody();
		btRigidBody* GetBody();
		void SetAddedToWorld(bool added);
		bool IsAddedToWorld() const;

		BodyType GetType() const;

		//手动设置刚体世界坐标（游戏逻辑驱动位置，如生成物体、动画运动学平台）
		void SetPosition(const glm::vec3& pos);
		glm::vec3 GetPosition() const;
		//手动设置刚体旋转四元数，逻辑同 SetPosition：
		void SetRotation(const glm::quat& rot);
		glm::quat GetRotation() const;


		void ApplyImpulse(const glm::vec3& impulse);
	private:
		//动力学刚体核心类，代表场景中受物理规律支配的实体物体
		//动力学刚体：btRigidBody，有质量，完全交由物理引擎模拟运动（箱子、石块、可抛掷道具）
		std::unique_ptr<btRigidBody> m_body;
		BodyType m_type = BodyType::Static;
		std::shared_ptr<Collider> m_collider;
		//质量，仅 Dynamic 生效，Static/Kinematic 质量强制 0
		float m_mass = 0.0f;
		//表面摩擦系数 0~1
		float m_friction = 0.5f;
		bool m_addedToWorld = false;
	};
}

