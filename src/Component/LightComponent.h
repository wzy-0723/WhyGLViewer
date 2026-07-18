#pragma once

#include "Component.h"

#include <glm/vec3.hpp>

namespace why
{
    class LightComponent : public Component
    {
        COMPONENT(LightComponent)
    public:
        void Update(float deltaTime) override;

        void SetColor(const glm::vec3& color);
        const glm::vec3& GetColor() const;

    private:
        glm::vec3 m_color = glm::vec3(1.0f);
    };
}