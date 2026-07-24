#pragma once
#include <array>
#include <qnamespace.h>
#include <glm/vec2.hpp>
#include <qevent.h>
#include <QMouseEvent>

namespace why
{
    class InputManager
    {
    public:
        InputManager() = default;
        InputManager(const InputManager&) = delete;
        InputManager(InputManager&&) = delete;
        InputManager& operator=(const InputManager&) = delete;
        InputManager& operator=(InputManager&&) = delete;

        bool HandleEvent(QEvent* e);
        void ListenMouseMove(const QPoint& point);

    public:


        void SetKeyPressed(Qt::Key key, bool pressed);
        bool IsKeyPressed(Qt::Key key);        

        void SetMouseButtonPressed(Qt::MouseButton button, bool pressed);
        bool IsMouseButtonPressed(Qt::MouseButton button);

        void SetMousePositionOld(const glm::vec2& pos);
        const glm::vec2& GetMousePositionOld() const;

        void SetMousePositionCurrent(const glm::vec2& pos);
        const glm::vec2& GetMousePositionCurrent() const;

        void SetMousePositionChanged(bool changed);
        bool IsMousePositionChanged() const;

    private:
        std::unordered_map<Qt::Key, bool> m_mapKey;
        std::unordered_map<Qt::MouseButton, bool> m_mapMouseButton;

        glm::vec2 m_mousePositionOld = glm::vec2(0.0f);
        glm::vec2 m_mousePositionCurrent = glm::vec2(0.0f);

        bool m_mousePositionChanged = false;

        friend class Engine;
    };
}