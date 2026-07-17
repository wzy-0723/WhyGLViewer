#include "CameraComponent.h"
#include "GameObject.h"
#include <glm/gtc/matrix_transform.hpp>
namespace why
{
    void CameraComponent::Update(float deltaTime)
    {
        
    }

    glm::mat4 CameraComponent::GetViewMatrix() const
    {
        // 视图变化等于持有组件者的model反变化，相当于相机往右移动，视野往左移动
        return glm::inverse(m_owner->GetWorldTransform());
    }

    glm::mat4 CameraComponent::GetProjectionMatrix(float aspect) const
    {
        return glm::perspective(glm::radians(m_fov), aspect, m_nearPlane, m_farPlane);

    }
}