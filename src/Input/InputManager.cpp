#include "InputManager.h"

namespace why
{
    bool InputManager::HandleEvent(QEvent* e)
    {
        if (e->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyEv = static_cast<QKeyEvent*>(e);
            int key = keyEv->key(); // 获取按键枚举 Qt::Key_XXX
            SetKeyPressed(static_cast<Qt::Key>(key), true);

            return true;
        }
        else if (e->type() == QEvent::KeyRelease)
        {
            QKeyEvent* keyEv = static_cast<QKeyEvent*>(e);
            int key = keyEv->key();
            SetKeyPressed(static_cast<Qt::Key>(key), false);
            return true;
        }
        else if (e->type() == QEvent::MouseButtonPress)
        {
            QMouseEvent* mouseButtonEv = static_cast<QMouseEvent*>(e);

            int mouseButton = mouseButtonEv->button();
            SetMouseButtonPressed(static_cast<Qt::MouseButton>(mouseButton), true);
            return true;
        }
        else if (e->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent* mouseButtonEv = static_cast<QMouseEvent*>(e);
            int mouseButton = mouseButtonEv->button();
            SetMouseButtonPressed(static_cast<Qt::MouseButton>(mouseButton), false);
            return true;
        }
        else if (e->type() == QEvent::MouseMove)
        {
            //QMouseEvent* mouseMoveEv = static_cast<QMouseEvent*>(e);
            //SetMousePositionOld(GetMousePositionCurrent());
            //glm::vec2 currentPos(static_cast<float>(mouseMoveEv->pos().x()), static_cast<float>(mouseMoveEv->pos().y()));            
            //SetMousePositionCurrent(currentPos);



        }


        return false;
    }

    void InputManager::ListenMouseMove(const QPoint& point)
    {
        SetMousePositionOld(GetMousePositionCurrent());
        glm::vec2 currentPos(static_cast<float>(point.x()), static_cast<float>(point.y()));
        SetMousePositionCurrent(currentPos);
    }

    void InputManager::SetKeyPressed(Qt::Key key, bool pressed)
    {
        m_mapKey[key] = pressed;

    }

    bool InputManager::IsKeyPressed(Qt::Key key)
    {
        return m_mapKey[key];
    }

    void InputManager::SetMouseButtonPressed(Qt::MouseButton button, bool pressed)
    {
        m_mapMouseButton[button] = pressed;
    }

    bool InputManager::IsMouseButtonPressed(Qt::MouseButton button)
    {
        return m_mapMouseButton[button];
    }

    void InputManager::SetMousePositionOld(const glm::vec2& pos)
    {
        m_mousePositionOld = pos;
    }

    const glm::vec2& InputManager::GetMousePositionOld() const
    {
        return m_mousePositionOld;
    }

    void InputManager::SetMousePositionCurrent(const glm::vec2& pos)
    {
        m_mousePositionCurrent = pos;
    }

    const glm::vec2& InputManager::GetMousePositionCurrent() const
    {
        return m_mousePositionCurrent;    
    }

    void InputManager::SetMousePositionChanged(bool changed)
    {
        m_mousePositionChanged = changed;
    }

    bool InputManager::IsMousePositionChanged() const
    {
        return m_mousePositionChanged;
    }

}