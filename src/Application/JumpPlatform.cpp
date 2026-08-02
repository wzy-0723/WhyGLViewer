#include "JumpPlatform.h"
#include "PhysicsComponent.h"
#include "KinematicCharacterController.h"

void JumpPlatform::Init()
{
    auto physics = GetComponent<why::PhysicsComponent>();
    if (physics)
    {
        auto rigidBody = physics->GetRigidBody();
        if (rigidBody)
        {
            rigidBody->AddContactListener(this);
        }
    }
}

void JumpPlatform::OnContact(
    why::CollisionObject* obj,
    const glm::vec3& pos,
    const glm::vec3& norm)
{
    if (obj->GetCollisionObjectType() == why::CollisionObjectType::KinematicCharacterController)
    {
        auto controller = static_cast<why::KinematicCharacterController*>(obj);
        if (controller)
        {
            controller->Jump(glm::vec3(0.0f, 20.0f, 0.0f));
        }
    }
}