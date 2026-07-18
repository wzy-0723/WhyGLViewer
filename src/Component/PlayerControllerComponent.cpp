#include "PlayerControllerComponent.h"
#include "InputManager.h"
#include "Engine.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec4.hpp>
#include <qevent.h>
#include "Logger.h"

namespace why
{
    void PlayerControllerComponent::Update(float deltaTime)
    {        
        auto inputManager = SINGLETON_PTR(Engine)->GetInputManager();
        auto rotation = m_owner->GetRotation();

        
        if (inputManager->IsMouseButtonPressed(Qt::MouseButton::LeftButton))
        {
            const auto& oldPos = inputManager->GetMousePositionOld();
            const auto& currentPos = inputManager->GetMousePositionCurrent();

            float deltaX = currentPos.x - oldPos.x;
            float deltaY = currentPos.y - oldPos.y;

            //LOG_INFO << "oldPos:("      << oldPos.x     << "," << oldPos.y      << ")";
            //LOG_INFO << "currentPos:("  << currentPos.x << "," << currentPos.y  << ")";

            // rot around Y axis
            float yAngle = -deltaX * m_sensitivity * deltaTime;
            glm::quat yRot = glm::angleAxis(yAngle, glm::vec3(0.0f, 1.0f, 0.0f));

            // rot around X axis
            float xAngle = -deltaY * m_sensitivity * deltaTime;
            glm::vec3 right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);
            glm::quat xRot = glm::angleAxis(xAngle, right);

            glm::quat deltaRot = yRot * xRot;
            rotation = glm::normalize(deltaRot * rotation);

            m_owner->SetRotation(rotation);
        }

        //// 右手坐标系，Z轴正方向与用户朝向相反，X正向为用户右向，Z正向为用户上向
        //// 相机right向与X轴正向相同，front向与Z正向相反，up向与Y正向相同。
        //// 以下运算是从单位矩阵中提取出单一方向，然后归一化
        //glm::vec3 front = glm::normalize(glm::vec3(rotMat * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));
        //glm::vec3 right = glm::normalize(glm::vec3(rotMat * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)));

        glm::vec3 front = rotation * glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 right = rotation * glm::vec3(1.0f, 0.0f, 0.0f);


        auto position = m_owner->GetPosition();

        // Left/Right movement
        // 以相机视角移动
        if (inputManager->IsKeyPressed(Qt::Key_A))
        {
            position -= right * m_moveSpeed * deltaTime;
        }
        else if (inputManager->IsKeyPressed(Qt::Key_D))
        {
            position += right * m_moveSpeed * deltaTime;
        }
        // Vertical movement
        if (inputManager->IsKeyPressed(Qt::Key_W))
        {
            position += front * m_moveSpeed * deltaTime;
        }
        else if (inputManager->IsKeyPressed(Qt::Key_S))
        {
            position -= front * m_moveSpeed * deltaTime;
        }
        m_owner->SetPosition(position);
    }
}