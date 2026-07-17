#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>
namespace why
{
    void GameObject::Update(float deltaTime)
    {
        for (auto& component : m_components)
        {
            component->Update(deltaTime);
        }

        for (auto it = m_children.begin(); it != m_children.end();)
        {
            if ((*it)->IsAlive())
            {
                (*it)->Update(deltaTime);
                ++it;
            }
            else
            {
                it = m_children.erase(it);
            }
        }
    }

    const std::string& GameObject::GetName() const
    {
        return m_name;
    }

    void GameObject::SetName(const std::string& name)
    {
        m_name = name;
    }

    GameObject* GameObject::GetParent()
    {
        return m_parent;
    }

    bool GameObject::IsAlive() const
    {
        return m_isAlive;
    }

    void GameObject::MarkForDestroy()
    {
        m_isAlive = false;
    }

    glm::mat4 GameObject::GetLocalTransform() const
    {
        glm::mat4 mat = glm::mat4(1.0f);

        // Translation
        mat = glm::translate(mat, m_position);

        // Rotation         x、y、z是弧度值
        mat = glm::rotate(mat, m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis
        mat = glm::rotate(mat, m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis
        mat = glm::rotate(mat, m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f)); // Z-axis

        // Scale
        mat = glm::scale(mat, m_scale);

        return mat;
    }

    glm::mat4 GameObject::GetWorldTransform() const
    {
        if (m_parent)
        {
            return m_parent->GetWorldTransform() * GetLocalTransform();
        }
        else
        {
            return GetLocalTransform();
        }
    }

    void GameObject::AddComponent(Component* component)
    {
        m_components.emplace_back(component);
        component->m_owner = this;
    }
}