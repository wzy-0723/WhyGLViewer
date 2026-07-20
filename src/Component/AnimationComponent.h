#pragma once
#include "Component.h"

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

namespace why
{

    struct KeyFrameVec3
    {
        float time = 0.0f;
        glm::vec3 value = glm::vec3(0.0f);
    };

    struct KeyFrameQuat
    {
        float time = 0.0f;
        glm::quat value = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    };
    // 位置、旋转和缩放信息
    struct TransformTrack
    {
        std::string targetName;
        std::vector<KeyFrameVec3> positions;
        std::vector<KeyFrameQuat> rotations;
        std::vector<KeyFrameVec3> scales;
    };

    struct AnimationClip
    {
        std::string name;
        float duration = 0.0f;
        bool looping = true;
        std::vector<TransformTrack> tracks;
    };

    // 约定哪个对象，有哪些tracks索引
    struct ObjectBinding
    {
        GameObject* object = nullptr;
        // 和AnimationClip的tracks索引对应，因为变化
        std::vector<size_t> trackIndices;           
    };

    class AnimationComponent : public Component
    {
        COMPONENT(AnimationComponent)
    public:
        void Update(float deltaTime) override;
        void SetClip(AnimationClip* clip);
        void RegisterClip(const std::string& name, const std::shared_ptr<AnimationClip>& clip);
        void Play(const std::string& name, bool loop = true);

    private:
        // 一个m_bindings只能对应一个动画，因此SetClip中要BuildBindings
        void BuildBindings();
        glm::vec3 Interpolate(const std::vector<KeyFrameVec3>& keys, float time);
        glm::quat Interpolate(const std::vector<KeyFrameQuat>& keys, float time);

    private:
        AnimationClip* m_clip = nullptr;
        float m_time = 0.0f;
        bool m_looping = true;
        bool m_isPlaying = false;

        std::unordered_map<std::string, std::shared_ptr<AnimationClip>> m_clips;
        // 决定动画帧如何组织？但是一个对象只能有一种动画？
        // 不是，m_clips已经表明能有多种动画，但是m_clip表明当前能播放的只有一种动画
        // 每种动画可能会携带子对象的动画一起，比如枪械shoot的fire效果和bullet运动，m_bindings要包含做动画的所有对象以及动画片段的变化索引
        std::unordered_map<GameObject*, std::unique_ptr<ObjectBinding>> m_bindings;
    };
}