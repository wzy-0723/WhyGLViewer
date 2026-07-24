#pragma once
#include "VertexLayout.h"
#include <memory>
#include <iostream>
#include <glm/vec3.hpp>
namespace why
{
    class Mesh
    {
    public:
        Mesh(const VertexLayout& layout, const std::vector<float>& vertices, const std::vector<uint32_t>& indices);
        Mesh(const VertexLayout& layout, const std::vector<float>& vertices);
        Mesh(const Mesh&) = delete;
        Mesh& operator=(const Mesh&) = delete;

        void Bind();
        void Draw();
        static std::shared_ptr<Mesh> CreateBox(const glm::vec3& extents = glm::vec3(1.0f));
        //static std::shared_ptr<Mesh> Load(const std::string& path);

    private:
        VertexLayout m_vertexLayout;
        GLuint m_VBO = 0;
        GLuint m_EBO = 0;
        GLuint m_VAO = 0;

        size_t m_vertexCout = 0;
        size_t m_indexCount = 0;
    };
}