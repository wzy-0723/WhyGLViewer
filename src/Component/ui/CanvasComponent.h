#pragma once

#include "Component.h"

namespace why
{
    class UIElementComponent;

    class CanvasComponent : public Component
    {
        COMPONENT(CanvasComponent)
    public:
        void Update(float deltaTime) override;
        void Render(UIElementComponent* element);
    };
}