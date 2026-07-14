#pragma once
#include <utility>

template <typename T>
class Singleton
{
public:
    // 可变参数模板：转发任意参数给 T 的构造函数
    template <typename... Args>
    static T* Instance(Args&&... args)
    {
        if (m_instance == nullptr)
        {
            // 完美转发参数，原地构造T对象
            m_instance = new T(std::forward<Args>(args)...);
        }
        return m_instance;
    }

    static void Clear()
    {
        if (m_instance != nullptr)
        {
            delete m_instance;
            m_instance = nullptr;
        }
    }

private:
    // 单例模板自身禁止实例化、拷贝
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    static T* m_instance;
};

template <typename T>
T* Singleton<T>::m_instance = nullptr;

// 宏兼容调用，无参/带参都可用
#ifndef SINGLETON_PTR
// 无参获取实例
#define SINGLETON_PTR(T) Singleton<T>::Instance()
// 带参版本宏（多参数逗号分隔）
#define SINGLETON_PTR_ARGS(T, ...) Singleton<T>::Instance(__VA_ARGS__)
#endif

#ifndef SINGLETON_CLEAR
#define SINGLETON_CLEAR(T) Singleton<T>::Clear()
#endif