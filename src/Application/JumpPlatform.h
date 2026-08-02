#pragma once

#include "GameObject.h"
#include "CollisionObject.h"
class JumpPlatform : public why::GameObject, public why::IContactListener
{
    GAMEOBJECT(JumpPlatform)
public:
    void Init() override;
    void OnContact(
        why::CollisionObject* obj,
        const glm::vec3& pos,
        const glm::vec3& norm) override;
};