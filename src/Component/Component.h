#pragma once
#include <string>
#include <unordered_map>
#include <memory>

#include <nlohmann/json.hpp>
namespace why
{
    class GameObject;

    class Component
    {
    public:
        virtual ~Component() = default;
        virtual void LoadProperties(const nlohmann::json& json);
        virtual void Update(float deltaTime);
        virtual void Init();
        virtual size_t GetTypeId() const = 0;
        GameObject* GetOwner();

        //对于同种T类型，返回的typeid相同，这样GameObj中能通过比对typeid并cast来获取对应实例
        template<typename T>
        static size_t StaticTypeId()
        {
            static size_t typeId = nextId++;
            return typeId;
        }

    protected:
        GameObject* m_owner = nullptr;

        friend class GameObject;

    private:
        static size_t nextId;
    };

    class ComponentCreatorBase
    {
    public:
        virtual ~ComponentCreatorBase() = default;
        virtual Component* CreateComponent() = 0;
    };

    template<typename T>
    class ComponentCreator : public ComponentCreatorBase
    {
    public:
        Component* CreateComponent() override
        {
            return new T();
        }
    };

    class ComponentFactory
    {
    public:
        static ComponentFactory& GetInstance();

        template<typename T>
        void RegisterComponent(const std::string& name)
        {
            m_creators.emplace(name, std::make_unique<ComponentCreator<T>>());
            m_parentMap[T::TypeId()].push_back(Component::StaticTypeId<Component>());
        }

        template<typename T, typename ParentType>
        void RegisterComponent(const std::string& name)
        {
            m_creators.emplace(name, std::make_unique<ComponentCreator<T>>());
            m_parentMap[T::TypeId()].push_back(Component::StaticTypeId<ParentType>());
        }

        bool HasParent(size_t objectType, size_t parentType);

        Component* CreateComponent(const std::string& name)
        {
            auto it = m_creators.find(name);
            if (it != m_creators.end())
            {
                return it->second->CreateComponent();
            }

            return nullptr;
        }

    private:
        std::unordered_map<std::string, std::unique_ptr<ComponentCreatorBase>> m_creators;
		std::unordered_map<size_t, std::vector<size_t>> m_parentMap;//当前类型的所有父类型的typeid集合
    };


#define COMPONENT(ComponentClass) \
public: \
    static size_t TypeId() { return why::Component::StaticTypeId<ComponentClass>(); } \
    size_t GetTypeId() const override { return TypeId(); } \
    static void Register() { why::ComponentFactory::GetInstance().RegisterComponent<ComponentClass>(std::string(#ComponentClass)); }

#define COMPONENT_2(ComponentClass, ParentComponentClass) \
public: \
    static size_t TypeId() { return why::Component::StaticTypeId<ComponentClass>(); } \
    size_t GetTypeId() const override { return TypeId(); } \
    static void Register() { why::ComponentFactory::GetInstance().RegisterComponent<ComponentClass, ParentComponentClass>(std::string(#ComponentClass)); }
};