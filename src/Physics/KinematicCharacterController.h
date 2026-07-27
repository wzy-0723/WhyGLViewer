#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

class btPairCachingGhostObject;
class btKinematicCharacterController;

namespace why
{
    class KinematicCharacterController
    {
    public:
        //KinematicCharacterController(float raduis, float height);
        KinematicCharacterController(float raduis, float height, const glm::vec3& position);
        ~KinematicCharacterController();

        glm::vec3 GetPosition() const;
        glm::quat GetRotation() const;

        void Walk(const glm::vec3& direction);
        void Jump(const glm::vec3& direction);

        //落地检测
        bool OnGround() const;

    private:
        // 胶囊柱体高度
        float m_height = 1.2f;
        // 胶囊半径
        float m_radius = 0.4f;

        // 碰撞幽灵体（角色碰撞胶囊载体）
        std::unique_ptr<btPairCachingGhostObject> m_ghost;
        // 角色运动控制器
        std::unique_ptr<btKinematicCharacterController> m_controller;
    }; 
}