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
        virtual void Init();
        virtual void LoadProperties(const nlohmann::json& json);
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
                if (component->GetTypeId() == typeId ||
                    ComponentFactory::GetInstance().HasParent(component->GetTypeId(), typeId))
                {
                    return static_cast<T*>(component.get());
                }
            }

            return nullptr;
        };
        GameObject* FindChildByName(const std::string& name);
        const std::vector<std::unique_ptr<GameObject>>& GetChildren() const;

        Scene* GetScene() { return  m_scene; };


        void SetActive(bool active) { m_active = active; };
        bool IsActive() { return m_active; };
        //3D
        const glm::vec3& GetPosition() const { return m_position; };
        glm::vec3 GetWorldPosition() const;
        void SetPosition(const glm::vec3& pos) { m_position = pos; };
        void SetWorldPosition(const glm::vec3& pos);
        const glm::quat& GetRotation() const { return m_rotation; };
        glm::quat GetWorldRotation();
        void SetRotation(const glm::quat& rot) { m_rotation = rot; };
        void SetWorldRotation(const glm::quat& rot);
        const glm::vec3& GetScale() const { return m_scale; };
        void SetScale(const glm::vec3& scale) { m_scale = scale; };

        glm::mat4 GetLocalTransform() const;
        glm::mat4 GetWorldTransform() const;

        //2D
        glm::vec2 GetPosition2D() const;
        glm::vec2 GetWorldPosition2D() const;
        void SetPosition2D(const glm::vec2& pos);
        float GetRotation2D() const;
        void SetRotation2D(float rotation);
        glm::vec2 GetScale2D() const;
        void SetScale2D(const glm::vec2& scale);
        glm::mat4 GetLocalTransform2D() const;
        glm::mat4 GetWorldTransform2D() const;

        // 相当于把材质和顶点数据加载放在一起，关注ParseGLTFNode函数
        static GameObject* LoadGLTF(const std::string& path);

        static GameObject* LoadGLTF(const std::string& path, Scene* scene);
    protected:
        GameObject() = default;

    protected:
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

    class ObjectCreatorBase
    {
    public:
        virtual ~ObjectCreatorBase() = default;
        virtual GameObject* CreateGameObject() = 0;
    };

    template<typename T>
    class ObjectCreator : public ObjectCreatorBase
    {
    public:
        virtual GameObject* CreateGameObject() override
        {
            return new T();
        }
    };

    class GameObjectFactory
    {
    public:
        static GameObjectFactory& GetInstance()
        {
            static GameObjectFactory instance;
            return instance;
        }

        template<typename T>
        void RegisterObject(const std::string& name)
        {
            m_creators.emplace(name, std::make_unique<ObjectCreator<T>>());
        }

        GameObject* CreateGameObject(const std::string& typeName)
        {
            auto it = m_creators.find(typeName);
            if (it == m_creators.end())
            {
                return nullptr;
            }

            return it->second->CreateGameObject();
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<ObjectCreatorBase>> m_creators;
    };

#define GAMEOBJECT(ObjectClass) \
public: \
    static void Register() { why::GameObjectFactory::GetInstance().RegisterObject<ObjectClass>(std::string(#ObjectClass)); }
}