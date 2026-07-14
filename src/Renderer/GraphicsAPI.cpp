#include "GraphicsAPI.h"
#include "Material.h"

#include <iostream>
#include "FileUtil.h"

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
}