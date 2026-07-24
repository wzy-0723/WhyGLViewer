#pragma once
#include "Component.h"
#include "RigidBody.h"

namespace why
{
    class PhysicsComponent : public Component
    {
        COMPONENT(PhysicsComponent)
    public:
        PhysicsComponent() = default;
        PhysicsComponent(const std::shared_ptr<RigidBody>& body);

        void Init() override;
        void Update(float deltaTime) override;

    private:
        std::shared_ptr<RigidBody> m_rigidBody;
    };
}