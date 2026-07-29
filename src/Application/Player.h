#pragma once
//#include <eng.h>
#include "GameObject.h"
#include "AnimationComponent.h"

class Player : public why::GameObject
{
    GAMEOBJECT(Player)
public:
    void Init() override;
    void Update(float deltaTime) override;

private:
    why::AnimationComponent* m_animationComponent = nullptr;
};
