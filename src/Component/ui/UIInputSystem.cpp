#include "UIInputSystem.h"
#include "CanvasComponent.h"
#include "UIElementComponent.h"
#include "Engine.h"


namespace why
{
    void UIInputSystem::SetActive(bool active)
    {
        m_active = active;
    }

    bool UIInputSystem::IsActive() const
    {
        return m_active;
    }

    void UIInputSystem::SetCanvas(CanvasComponent* canvas)
    {
        m_activeCanvas = canvas;
    }

    void UIInputSystem::Update(float deltaTime)
    {
        if (!m_active || !m_activeCanvas)
        {
            return;
        }
        
        auto input = SINGLETON_PTR(Engine)->GetInputManager();
        bool mouseDown = input->IsMouseButtonPressed(Qt::MouseButton::LeftButton);
        bool mousePressed = input->WasMouseButtonPressed(Qt::MouseButton::LeftButton);
        bool mouseReleased = input->WasMouseButtonReleased(Qt::MouseButton::LeftButton);

        auto mousePos = input->GetMousePositionCurrent();
        mousePos.y = SINGLETON_PTR(GraphicsAPI)->GetViewport().height - mousePos.y;

        UIElementComponent* hit = nullptr;
        auto uiElements = CollectUI(m_activeCanvas);
        for (auto element : uiElements)
        {
            if (element->HitTest(mousePos))
            {
                hit = element;
                break;
            }
        }

        if (hit != m_hovered)
        {
            if (m_hovered)
            {
                m_hovered->OnPointerExit();
            }

            m_hovered = hit;

            if (m_hovered)
            {
                m_hovered->OnPointerEnter();
            }
            m_pressed = nullptr;
        }

        if (!m_pressed)
        {
            if (mousePressed && m_hovered)
            {
                m_pressed = m_hovered;
                m_pressed->OnPointerDown();
            }
        }

        if (mouseReleased)
        {
            if (m_pressed)
            {
                m_pressed->OnPointerUp();

                if (m_pressed == m_hovered)
                {
                    m_pressed->OnClick();
                }
            }

            m_pressed = nullptr;
        }
    }

    std::vector<UIElementComponent*> UIInputSystem::CollectUI(CanvasComponent* canvas)
    {
        std::vector<UIElementComponent*> result;
        GameObject* canvasObject = canvas->GetOwner();
        const auto& children = canvasObject->GetChildren();

        for (const auto& child : children)
        {
            if (auto component = child->GetComponent<UIElementComponent>())
            {
                canvas->CollectUI(component, result);
            }
        }
        return result;
    }

}