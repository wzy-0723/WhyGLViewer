#include "Collider.h"

#include <btBulletCollisionCommon.h>

namespace why
{
    Collider::~Collider()
    {
        if (m_shape)
        {
            delete m_shape;
        }
    }

    btCollisionShape* Collider::GetShape()
    {
        return m_shape;
    }

    BoxCollider::BoxCollider(const glm::vec3& extents)
    {
        glm::vec3 halfExtents = extents * 0.5f;
        //box长宽高
        m_shape = new btBoxShape(btVector3(halfExtents.x, halfExtents.y, halfExtents.z));
    }

    SphereCollider::SphereCollider(float radius)
    {
        // 球体半径
        m_shape = new btSphereShape(radius);
    }

    CapsuleCollider::CapsuleCollider(float radius, float height)
    {
        // 胶囊体由圆筒和两个半球构成，圆筒长和球半径
        m_shape = new btCapsuleShape(btScalar(radius), btScalar(height));
    }
}