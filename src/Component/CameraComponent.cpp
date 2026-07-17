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
        glm::mat4 mat = glm::mat4(1.0f);
        mat = glm::mat4_cast(m_owner->GetRotation());

        // 推入平移
        mat[3] = glm::vec4(m_owner->GetPosition(), 1.0f);

        if (m_owner->GetParent())
        {
            mat = m_owner->GetParent()->GetWorldTransform() * mat;
        }

        return glm::inverse(mat);


        //GetWorldTransform已经是四元数表达，不是很明白为啥还要写上面的逻辑@why.todo
        //// 视图变化等于持有组件者的model反变化，相当于相机往右移动，视野往左移动
        //return glm::inverse(m_owner->GetWorldTransform());
    }

    glm::mat4 CameraComponent::GetProjectionMatrix(float aspect) const
    {
        return glm::perspective(glm::radians(m_fov), aspect, m_nearPlane, m_farPlane);

    }
}