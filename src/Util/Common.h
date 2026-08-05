#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace why
{
    struct CameraData
    {
        glm::mat4 viewMatrix;
        glm::mat4 projectionMatrix;
        glm::vec3 position;
        glm::mat4 orthoMatrix;
    };

    struct LightData
    {
        glm::vec3 color;
        glm::vec3 position;
    };
}