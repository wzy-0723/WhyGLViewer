#pragma once
//#include <eng.h>
#include "GameObject.h"
#include "AnimationComponent.h"
#include "AudioComponent.h"
#include "PlayerControllerComponent.h"
class Player : public why::GameObject
{
    GAMEOBJECT(Player)
public:
    void Init() override;
    void Update(float deltaTime) override;

private:
    why::AnimationComponent* m_animationComponent = nullptr;
    why::AudioComponent* m_audioComponent = nullptr;
    why::PlayerControllerComponent* m_playerControllerComponent = nullptr;
};
