#pragma once
#include <array>
#include <qnamespace.h>
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

    public:
        void SetKeyPressed(Qt::Key key, bool pressed);
        bool IsKeyPressed(Qt::Key key);        
        std::unordered_map<Qt::Key, bool> m_mapKey;
    private:
    };
}