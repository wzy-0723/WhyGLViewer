#include "Component.h"

namespace why
{
    size_t Component::nextId = 1;

    void Component::Init()
    {
    }

    GameObject* Component::GetOwner()
    {
        return m_owner;
    }
}