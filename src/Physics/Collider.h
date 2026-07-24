#pragma once

#include <glm/vec3.hpp>

class btCollisionShape;

namespace why
{
    class Collider
    {
    public:
        virtual ~Collider();
        btCollisionShape* GetShape();

    protected:
        btCollisionShape* m_shape = nullptr;
    };

    //
    class BoxCollider : public Collider
    {
    public:
        BoxCollider(const glm::vec3& extents);
    };

    //球体碰撞体
    class SphereCollider : public Collider
    {
    public:
        SphereCollider(float radius);
    };

    //胶囊碰撞体构造
    class CapsuleCollider : public Collider
    {
    public:
        CapsuleCollider(float radius, float height);
    };
}