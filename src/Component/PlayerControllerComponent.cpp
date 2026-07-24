#include "PlayerControllerComponent.h"
#include "InputManager.h"
#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec4.hpp>
#include <qevent.h>
#include "Logger.h"

namespace why
{
    void PlayerControllerComponent::Init()
    {
        m_kinematicController = std::make_unique<KinematicCharacterController>(0.4f, 1.2f);
    }   

    void PlayerControllerComponent::Update(float deltaTime)
    {        
        auto inputManager = SINGLETON_PTR(Engine)->GetInputManager();
        auto rotation = m_owner->GetRotation();

        
        //if (inputManager->IsMouseButtonPressed(Qt::MouseButton::LeftButton))
        {
            const auto& oldPos = inputManager->GetMousePositionOld();
            const auto& currentPos = inputManager->GetMousePositionCurrent();

            float deltaX = currentPos.x - oldPos.x;
            float deltaY = currentPos.y - oldPos.y;

            if (deltaX > 200 || deltaY > 200)
            {
                deltaX = 0;
                deltaY = 0;
            }

            LOG_INFO << "oldPos:("      << oldPos.x     << "," << oldPos.y      << ")";
            LOG_INFO << "currentPos:("  << currentPos.x << "," << currentPos.y  << ")";

            // rot around Y axis
            float yDeltaAngle = -deltaX * m_sensitivity * deltaTime;
            m_yRot += yDeltaAngle;
            glm::quat yRot = glm::angleAxis(glm::radians(m_yRot), glm::vec3(0.0f, 1.0f, 0.0f));

            // rot around X axis
            float xDeltaAngle = -deltaY * m_sensitivity * deltaTime;
            m_xRot += xDeltaAngle;
            m_xRot = std::clamp(m_xRot, -89.0f, 89.0f);
            glm::quat xRot = glm::angleAxis(glm::radians(m_xRot), glm::vec3(1.0f, 0.0f, 0.0f));

            rotation = glm::normalize(yRot * xRot);

            m_owner->SetRotation(rotation);
        }

        //// 右手坐标系，Z轴正方向与用户朝向相反，X正向为用户右向，Z正向为用户上向
        //// 相机right向与X轴正向相同，front向与Z正向相反，up向与Y正向相同。
        //// 以下运算是从单位矩阵中提取出单一方向，然后归一化
        //glm::vec3 front = glm::normalize(glm::vec3(rotMat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
        //glm::vec3 right = glm::normalize(glm::vec3(rotMat * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));

        glm::vec3 front = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);


        glm::vec3 move(0.0f);

        // Left/Right movement
        // 以相机视角移动
        if (inputManager->IsKeyPressed(Qt::Key_A))
        {
            move -= right;
        }
        else if (inputManager->IsKeyPressed(Qt::Key_D))
        {
            move += right;
        }
        // Vertical movement
        if (inputManager->IsKeyPressed(Qt::Key_W))
        {
            move += front;
        }
        else if (inputManager->IsKeyPressed(Qt::Key_S))
        {
            move -= front;
        }
        if (inputManager->IsKeyPressed(Qt::Key_Space))
        {
            m_kinematicController->Jump(glm::vec3(0.0f, 5.0f, 0.0f));
        }

        if (glm::dot(move, move) > 0)
        {
            move = glm::normalize(move);
        }
        m_kinematicController->Walk(move * m_moveSpeed * deltaTime);

        m_owner->SetPosition(m_kinematicController->GetPosition());
    }
}