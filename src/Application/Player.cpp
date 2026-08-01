#include "Player.h"
#include "CameraComponent.h"
#include "PlayerControllerComponent.h"
#include "Engine.h"

void Player::Init()
{
    //场景加载之前
    //AddComponent(new why::CameraComponent());
    //SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    //AddComponent(new why::PlayerControllerComponent());

    //auto gun = why::GameObject::LoadGLTF("Models/sten_gunmachine_carbine/scene.gltf");
    //gun->SetParent(this);
    //gun->SetPosition(glm::vec3(0.75f, -0.5f, -0.75f));
    //gun->SetScale(glm::vec3(-1.0f, 1.0f, 1.0f));

    //if (auto anim = gun->GetComponent<why::AnimationComponent>())
    //{
    //    if (auto bullet = gun->FindChildByName("bullet_33"))
    //    {
    //        bullet->SetActive(false);
    //    }

    //    if (auto fire = gun->FindChildByName("BOOM_35"))
    //    {
    //        fire->SetActive(false);
    //    }

    //    anim->Play("shoot", false);
    //}
    //m_animationComponent = gun->GetComponent<why::AnimationComponent>();

    // 
    if (auto bullet = FindChildByName("bullet_33"))
    {
        bullet->SetActive(false);
    }

    if (auto fire = FindChildByName("BOOM_35"))
    {
        fire->SetActive(false);
    }

    if (auto gun = FindChildByName("Gun"))
    {
        m_animationComponent = gun->GetComponent<why::AnimationComponent>();
    }


    m_audioComponent = GetComponent<why::AudioComponent>();
    m_playerControllerComponent = GetComponent<why::PlayerControllerComponent>();
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

                        if (m_audioComponent)
            {
                if (m_audioComponent->IsPlaying("shoot"))
                {
                    m_audioComponent->Stop("shoot");
                }
                m_audioComponent->Play("shoot");
            }
        }
    }
    
    if (input->IsKeyPressed(Qt::Key::Key_Space))
    {
        if (m_audioComponent && !m_audioComponent->IsPlaying("jump"))
        {
            m_audioComponent->Play("jump");
        }
    }

    bool walking =
        input->IsKeyPressed(Qt::Key::Key_W) ||
        input->IsKeyPressed(Qt::Key::Key_A) ||
        input->IsKeyPressed(Qt::Key::Key_S) ||
        input->IsKeyPressed(Qt::Key::Key_D);

    if (walking && m_playerControllerComponent && m_playerControllerComponent->OnGround())
    {
        if (m_audioComponent && !m_audioComponent->IsPlaying("step"))
        {
            m_audioComponent->Play("step", true);
        }
    }
    else
    {
        if (m_audioComponent && m_audioComponent->IsPlaying("step"))
        {
            m_audioComponent->Stop("step");

        }
    }
}