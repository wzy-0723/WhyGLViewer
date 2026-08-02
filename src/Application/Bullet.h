#pragma once

#include "GameObject.h"

class Bullet : public why::GameObject
{
    GAMEOBJECT(Bullet)
public:
    void Update(float deltaTime) override;

private:
    float m_lifetime = 2.0f;
};