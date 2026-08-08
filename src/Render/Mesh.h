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
        //Mesh(const Mesh&) = delete;
        //Mesh& operator=(const Mesh&) = delete;
        ~Mesh();
        void Bind();
        void Unbind();
        void Draw();
        static std::shared_ptr<Mesh> CreateBox(const glm::vec3& extents = glm::vec3(1.0f));
        /*
        radius：球体半径
        sectors：水平分段数（环绕 Z 轴一圈的切分份数，横向分片）
        stacks：垂直分段数（南北极之间纵向切分份数，纵向分片）       
        */
        static std::shared_ptr<Mesh> CreateSphere(float radius, int sectors, int stacks);
        //static std::shared_ptr<Mesh> Load(const std::string& path);
        static std::shared_ptr<Mesh> CreatePlane();


        //动态绘制
        void DrawIndexedRange(uint32_t startIndex, uint32_t indexCount);
        void UpdateDynamic(const std::vector<float>& vertices);
        void UpdateDynamic(const std::vector<float>& vertices, const std::vector<uint32_t>& indices);

    private:
        VertexLayout m_vertexLayout;
        GLuint m_VBO = 0;
        GLuint m_EBO = 0;
        GLuint m_VAO = 0;

        size_t m_vertexCout = 0;
        size_t m_indexCount = 0;
    };
}