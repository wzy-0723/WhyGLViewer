#pragma once

#include <memory>

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

namespace why
{
    class RigidBody;

    class PhysicsManager
    {
    public:
        PhysicsManager();
        ~PhysicsManager();

        void Init();
        void Update(float deltaTime);
        //添加刚体
        void AddRigidBody(RigidBody* body);
        //移除刚体
        void RemoveRigidBody(RigidBody* body);

        //获取原生物理世界 
        btDiscreteDynamicsWorld* GetWorld();

    private:
        std::unique_ptr<btBroadphaseInterface> m_broadphase;
        std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfig;
        std::unique_ptr<btCollisionDispatcher> m_dispatcher;
        std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
        std::unique_ptr<btDiscreteDynamicsWorld> m_world;
    };
}