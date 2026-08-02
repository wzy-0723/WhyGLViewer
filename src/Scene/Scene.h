#pragma once
#include "GameObject.h"
#include <vector>
#include <string>
#include <memory>
#include "Common.h"
namespace why
{
    class Scene
    {
    public:
        static void RegisterTypes();
        void Update(float deltaTime);
        void Clear();

        GameObject* CreateObject(const std::string& name, GameObject* parent = nullptr);
        GameObject* CreateObject(const std::string& type, const std::string& name, GameObject* parent = nullptr);

        template<typename T, typename = typename std::enable_if_t<std::is_base_of_v<GameObject, T>>>
        T* CreateObject(const std::string& name, GameObject* parent = nullptr)
        {
            auto obj = new T();
            obj->SetName(name);
            obj->m_scene = this;

            if (m_isUpdating)
            {
                m_objectsToAdd.push_back({ obj, parent });
            }
            else
            {
                SetParent(obj, parent);
            }
            return obj;
        }

        bool SetParent(GameObject* obj, GameObject* parent);

        void SetMainCamera(GameObject* camera);
        GameObject* GetMainCamera();

        std::vector<LightData> CollectLights();

        static std::shared_ptr<Scene> Load(const std::string& path);

    private:
        void CollectLightsRecursive(GameObject* obj, std::vector<LightData>& out);
        void LoadObject(const nlohmann::json& jsonObject, GameObject* parent);
    private:
        std::vector<std::unique_ptr<GameObject>> m_objects;

		// 添加子弹逻辑相关，在更新循环中添加对象时，避免直接修改 m_objects，防止迭代器失效
        std::vector<std::pair<GameObject*, GameObject*>> m_objectsToAdd;
        bool m_isUpdating = false;


        GameObject* m_mainCamera = nullptr;
    };
}