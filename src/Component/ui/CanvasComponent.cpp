#include "CanvasComponent.h"
#include "UIElementComponent.h"
#include "GameObject.h"

namespace why
{
    void CanvasComponent::Update(float deltaTime)
    {
        const auto& children = m_owner->GetChildren();
        for (const auto& child : children)
        {
            if (auto comp = child->GetComponent<UIElementComponent>())
            {
                Render(comp);
            }
        }
    }

    void CanvasComponent::Render(UIElementComponent* element)
    {
        if (!element)
        {
            return;
        }

        element->Render(this);

        const auto& children = element->GetOwner()->GetChildren();
        for (const auto& child : children)
        {
            if (auto comp = child->GetComponent<UIElementComponent>())
            {
                Render(comp);
            }
        }
    }
}