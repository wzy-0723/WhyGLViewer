#pragma once
#include <string>
#include <vector>
#include <memory>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "Component.h"
#include <glm/gtc/quaternion.hpp>
namespace why
{
    class Scene;

    class GameObject
    {
    private:

    public:
        virtual ~GameObject() = default;
        virtual void Update(float deltaTime);
        const std::string& GetName() const;
        void SetName(const std::string& name);
        GameObject* GetParent();
        bool SetParent(GameObject* parent);       
        bool IsAlive() const;
        void MarkForDestroy();

        void AddComponent(Component* component);
        template<typename T, typename = typename std::enable_if_t<std::is_base_of_v<Component, T>>>
        T* GetComponent()
        {
            size_t typeId = Component::StaticTypeId<T>();

            for (auto& component : m_components)
            {
                if (component->GetTypeId() == typeId)
                {
                    return static_cast<T*>(component.get());
                }
            }

            return nullptr;
        };
        GameObject* FindChildByName(const std::string& name);


        Scene* GetScene() { return  m_scene; };


        void SetActive(bool active) { m_active = active; };
        bool IsActive() { return m_active; };

        const glm::vec3& GetPosition() const { return m_position; };
        glm::vec3 GetWorldPosition() const;
        void SetPosition(const glm::vec3& pos) { m_position = pos; };

        const glm::quat& GetRotation() const { return m_rotation; };
        void SetRotation(const glm::quat& rot) { m_rotation = rot; };

        const glm::vec3& GetScale() const { return m_scale; };
        void SetScale(const glm::vec3& scale) { m_scale = scale; };

        glm::mat4 GetLocalTransform() const;
        glm::mat4 GetWorldTransform() const;

        // 相当于把材质和顶点数据加载放在一起，关注ParseGLTFNode函数
        static GameObject* LoadGLTF(const std::string& path);

    protected:
        GameObject() = default;

    private:
        std::string m_name;
        GameObject* m_parent = nullptr;
        Scene* m_scene = nullptr;
        std::vector<std::unique_ptr<GameObject>> m_children;
        std::vector<std::unique_ptr<Component>> m_components;


        bool m_isAlive = true;
        glm::vec3 m_position = glm::vec3(0.0f);
        glm::quat m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        glm::vec3 m_scale = glm::vec3(1.0f);
        bool m_active = true;

        friend class Scene;
    };
}