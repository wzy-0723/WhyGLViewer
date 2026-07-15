#include "Mesh.h"
#include "GraphicsAPI.h"
#include "OpenGLFunc.h"

namespace why
{
    Mesh::Mesh(const VertexLayout& layout, const std::vector<float>& vertices, const std::vector<uint32_t>& indices)
    {
        m_vertexLayout = layout;

        m_VBO = SINGLETON_PTR(GraphicsAPI)->CreateVertexBuffer(vertices);
        m_EBO = SINGLETON_PTR(GraphicsAPI)->CreateIndexBuffer(indices);

        OPENGLFUNC->glGenVertexArrays(1, &m_VAO);
        OPENGLFUNC->glBindVertexArray(m_VAO);

        OPENGLFUNC->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        for (auto& element : m_vertexLayout.elements)
        {
            OPENGLFUNC->glVertexAttribPointer(element.index, element.size, element.type, GL_FALSE,
                m_vertexLayout.stride, (void*)(uintptr_t)element.offset);
            OPENGLFUNC->glEnableVertexAttribArray(element.index);
        }

        OPENGLFUNC->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

        OPENGLFUNC->glBindVertexArray(0);
        OPENGLFUNC->glBindBuffer(GL_ARRAY_BUFFER, 0);
        OPENGLFUNC->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        m_vertexCout = (vertices.size() * sizeof(float)) / m_vertexLayout.stride;
        m_indexCount = indices.size();
    }

    Mesh::Mesh(const VertexLayout& layout, const std::vector<float>& vertices)
    {
        m_vertexLayout = layout;

        m_VBO = SINGLETON_PTR(GraphicsAPI)->CreateVertexBuffer(vertices);

        OPENGLFUNC->glGenVertexArrays(1, &m_VAO);
        OPENGLFUNC->glBindVertexArray(m_VAO);

        OPENGLFUNC->glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        for (auto& element : m_vertexLayout.elements)
        {
            OPENGLFUNC->glVertexAttribPointer(element.index, element.size, element.type, GL_FALSE,
                m_vertexLayout.stride, (void*)(uintptr_t)element.offset);
            OPENGLFUNC->glEnableVertexAttribArray(element.index);
        }

        OPENGLFUNC->glBindVertexArray(0);
        OPENGLFUNC->glBindBuffer(GL_ARRAY_BUFFER, 0);

        m_vertexCout = (vertices.size() * sizeof(float)) / m_vertexLayout.stride;
    }

    void Mesh::Bind()
    {
        OPENGLFUNC->glBindVertexArray(m_VAO);
    }

    void Mesh::Draw()
    {
        if (m_indexCount > 0)
        {
            OPENGLFUNC->glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
        }
        else
        {
            OPENGLFUNC->glDrawArrays(GL_TRIANGLES, 0, m_vertexCout);
        }
    }
}