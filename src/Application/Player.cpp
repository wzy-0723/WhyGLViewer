#include "Player.h"
#include "CameraComponent.h"
#include "PlayerControllerComponent.h"
#include "Engine.h"

void Player::Init()
{
    AddComponent(new why::CameraComponent());
    SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    AddComponent(new why::PlayerControllerComponent());

    auto gun = why::GameObject::LoadGLTF("Models/sten_gunmachine_carbine/scene.gltf");
    gun->SetParent(this);
    gun->SetPosition(glm::vec3(0.75f, -0.5f, -0.75f));
    gun->SetScale(glm::vec3(-1.0f, 1.0f, 1.0f));

    if (auto anim = gun->GetComponent<why::AnimationComponent>())
    {
        if (auto bullet = gun->FindChildByName("bullet_33"))
        {
            bullet->SetActive(false);
        }

        if (auto fire = gun->FindChildByName("BOOM_35"))
        {
            fire->SetActive(false);
        }

        anim->Play("shoot", false);
    }
    m_animationComponent = gun->GetComponent<why::AnimationComponent>();
}

void Player::Update(float deltaTime)
{
    why::GameObject::Update(deltaTime);
    
    auto input = SINGLETON_PTR(why::Engine)->GetInputManager();
    if (input->IsMouseButtonPressed(Qt::MouseButton::LeftButton))
    {
        if (m_animationComponent && !m_animationComponent->IsPlaying())
        {
            m_animationComponent->Play("shoot", false);
        }
    }
}