#include "Bullet.h"

void Bullet::Update(float deltaTime)
{
    why::GameObject::Update(deltaTime);
    m_lifetime -= deltaTime;
    if (m_lifetime <= 0.0f)
    {
        MarkForDestroy();
    }
}