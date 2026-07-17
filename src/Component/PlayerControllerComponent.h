#pragma once 

#include "Component.h"

namespace why
{
    class PlayerControllerComponent : public Component
    {
        COMPONENT(PlayerControllerComponent)

    public:
        void Update(float deltaTime) override;

    private:
        float m_sensitivity = 0.05f;
        float m_moveSpeed = 1.0f;
    };
}