#include "Scene.h"

namespace why
{
    void Scene::Update(float deltaTime)
    {
        for (auto it = m_objects.begin(); it != m_objects.end();)
        {
            if ((*it)->IsAlive())
            {
                (*it)->Update(deltaTime);
                ++it;
            }
            else
            {
                it = m_objects.erase(it);
            }
        }
    }

    void Scene::Clear()
    {
        m_objects.clear();
    }

    GameObject* Scene::CreateObject(const std::string& name, GameObject* parent)
    {
        auto obj = new GameObject();
        obj->SetName(name);
        SetParent(obj, parent);
        return obj;
    }

    bool Scene::SetParent(GameObject* obj, GameObject* parent)
    {
        bool result = false;
        auto currentParent = obj->GetParent();

        // 要将父节点置为空的情况，也就是需要添加到m_objects
        if (parent == nullptr)
        {
            // 将有父节点的子节点变成无父节点的根节点，需要添加到m_objects
            if (currentParent != nullptr)
            {
                // 在父节点的子节点集合中定位当前对象
                auto it = std::find_if(
                    currentParent->m_children.begin(),
                    currentParent->m_children.end(),
                    [obj](const std::unique_ptr<GameObject>& el) {
                        return el.get() == obj;
                    }
                );

                // 如果定位到，从父结点中移除这个obj位置，将obj添加到当前场景？
                if (it != currentParent->m_children.end())
                {
                    m_objects.push_back(std::move(*it));
                    obj->m_parent = nullptr;
                    currentParent->m_children.erase(it);
                    result = true;
                }
            }
            // 刚创建的无父节点的节点，或者已有的无父结点的节点
            else
            {
                auto it = std::find_if(
                    m_objects.begin(),
                    m_objects.end(),
                    [obj](const std::unique_ptr<GameObject>& el) {
                        return el.get() == obj;
                    }
                );

                if (it == m_objects.end())
                {
                    std::unique_ptr<GameObject> objHolder(obj);
                    m_objects.push_back(std::move(objHolder));
                    result = true;
                }
            }
        }
        // 要给节点添加父节点的情况
        else
        {
            // 已有父节点
            if (currentParent != nullptr)
            {
                auto it = std::find_if(
                    currentParent->m_children.begin(),
                    currentParent->m_children.end(),
                    [obj](const std::unique_ptr<GameObject>& el) {
                        return el.get() == obj;
                    }
                );

                if (it != currentParent->m_children.end())
                {
                    bool found = false;
                    auto currentElement = parent;
                    while (currentElement)
                    {
                        if (currentElement == obj)
                        {
                            found = true;
                            break;
                        }
                        currentElement = currentElement->GetParent();
                    }
                    // 防止祖父节点中有自身的情况
                    if (!found)
                    {
                        parent->m_children.push_back(std::move(*it));
                        obj->m_parent = parent;
                        currentParent->m_children.erase(it);
                        result = true;
                    }
                }
            }
            // No parent currently. This can be in 2 cases
            // 1. The object is in the scene root
            // 2. The object has been just created
            else
            {
                auto it = std::find_if(
                    m_objects.begin(),
                    m_objects.end(),
                    [obj](const std::unique_ptr<GameObject>& el) {
                        return el.get() == obj;
                    }
                );

                // The object has been hust created
                // 在根节点m_objects中
                if (it == m_objects.end())
                {
                    std::unique_ptr<GameObject> objHolder(obj);
                    parent->m_children.push_back(std::move(objHolder));
                    obj->m_parent = parent;
                    result = true;
                }
                else
                {
                    // 在根节点m_objects中
                    // 移除出m_objects，变为节点的子节点
                    bool found = false;
                    auto currentElement = parent;
                    while (currentElement)
                    {
                        if (currentElement == obj)
                        {
                            found = true;
                            break;
                        }
                        currentElement = currentElement->GetParent();
                    }

                    if (!found)
                    {
                        parent->m_children.push_back(std::move(*it));
                        obj->m_parent = parent;
                        m_objects.erase(it);
                        result = true;
                    }
                }
            }
        }

        return result;
    }

    void Scene::SetMainCamera(GameObject* camera)
    {
        m_mainCamera = camera;
    }

    GameObject* Scene::GetMainCamera()
    {
        return m_mainCamera;
    }
}