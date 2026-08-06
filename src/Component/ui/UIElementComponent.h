#pragma once

#include "Component.h"

#include <glm/vec2.hpp>

namespace why
{
    class CanvasComponent;

    class UIElementComponent : public Component
    {
        COMPONENT(UIElementComponent)
    public:
        virtual void Render(CanvasComponent* canvas);

    protected:
        glm::vec2 m_pivot = glm::vec2(0.5f);
    };
}