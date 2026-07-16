#include "InputManager.h"

namespace why
{
    void InputManager::SetKeyPressed(Qt::Key key, bool pressed)
    {
        m_mapKey[key] = pressed;

    }

    bool InputManager::IsKeyPressed(Qt::Key key)
    {
        return m_mapKey[key];
    }
}