#include "GraphicsAPI.h"
#include "Material.h"

#include <iostream>
#include "FileUtil.h"
#include "OpenGLFunc.h"
namespace why
{
    std::shared_ptr<QOpenGLShaderProgram> GraphicsAPI::CreateShaderProgram(const std::string& strShaderName)
    {
        std::shared_ptr<QOpenGLShaderProgram> pShader = std::make_shared<QOpenGLShaderProgram>();

        SINGLETON_PTR(PathAppender)->SetSourcePath(":/Shader");
        std::string strVertexShaderPath = SINGLETON_PTR(PathAppender)->AppendChildPath(strShaderName + ".vert").GetPath();
        
        SINGLETON_PTR(PathAppender)->SetSourcePath(":/Shader");
        std::string strFragmentShaderPath = SINGLETON_PTR(PathAppender)->AppendChildPath(strShaderName + ".frag").GetPath();

        bool result = true;
        result = pShader->addShaderFromSourceFile(QOpenGLShader::Vertex, QString::fromStdString(strVertexShaderPath));
        if (!result) {
            qDebug() << pShader->log();
        }
        result = pShader->addShaderFromSourceFile(QOpenGLShader::Fragment, QString::fromStdString(strFragmentShaderPath));
        if (!result) {
            qDebug() << pShader->log();
        }
        result = pShader->link();
        if (!result) {
            qDebug() << pShader->log();
        }

        return pShader;
    }

    void GraphicsAPI::BindMaterial(Material* material)
    {
        if (material)
        {
            material->Bind();
        }
    }

    GLuint GraphicsAPI::CreateVertexBuffer(const std::vector<float>& vertices)
    {
        GLuint VBO = 0;
        OPENGLFUNC->glGenBuffers(1, &VBO);
        OPENGLFUNC->glBindBuffer(GL_ARRAY_BUFFER, VBO);
        OPENGLFUNC->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        OPENGLFUNC->glBindBuffer(GL_ARRAY_BUFFER, 0);
        return VBO;
    }

    GLuint GraphicsAPI::CreateIndexBuffer(const std::vector<uint32_t>& indices)
    {
        GLuint EBO = 0;
        OPENGLFUNC->glGenBuffers(1, &EBO);
        OPENGLFUNC->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        OPENGLFUNC->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
        OPENGLFUNC->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        return EBO;
    }

}