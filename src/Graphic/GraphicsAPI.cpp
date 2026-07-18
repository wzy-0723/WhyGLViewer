#include "GraphicsAPI.h"
#include "Material.h"

#include <iostream>
#include "FileUtil.h"
#include "OpenGLFunc.h"
namespace why
{
    bool GraphicsAPI::Init()
    {
        // 保存像素的Z深度，因为屏幕实际显示2维信息，当重叠时开启深度测试才能正确显示当前视角下的像素颜色
        OPENGLFUNC->glEnable(GL_DEPTH_TEST);
        return true;
    }

    std::shared_ptr<ShaderProgram> GraphicsAPI::CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource)
    {
        
        GLuint vertexShader = OPENGLFUNC->glCreateShader(GL_VERTEX_SHADER);
        const char* vertexShaderCStr = vertexSource.c_str();
        GLCall(glShaderSource(vertexShader, 1, &vertexShaderCStr, nullptr));
        GLCall(glCompileShader(vertexShader));

        GLint success;
        GLCall(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success));
        if (!success)
        {
            char infoLog[512];
            GLCall(glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog));
            LOG_ERROR << "ERROR:VERTEX_SHADER_COMPILATION_FAILED: " << infoLog << std::endl;
            return nullptr;
        }
        
        GLuint fragmentShader = OPENGLFUNC->glCreateShader(GL_FRAGMENT_SHADER);
        const char* fragmentShaderSourceCStr = fragmentSource.c_str();
        GLCall(glShaderSource(fragmentShader, 1, &fragmentShaderSourceCStr, nullptr));
        GLCall(glCompileShader(fragmentShader));

        GLCall(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success));
        if (!success)
        {
            char infoLog[512];
            GLCall(glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog));
            LOG_ERROR << "ERROR:FRAGMENT_SHADER_COMPILATION_FAILED: " << infoLog << std::endl;
            return nullptr;
        }
        
        GLuint shaderProgramID = OPENGLFUNC->glCreateProgram();
        GLCall(glAttachShader(shaderProgramID, vertexShader));
        GLCall(glAttachShader(shaderProgramID, fragmentShader));
        GLCall(glLinkProgram(shaderProgramID));

        GLCall(glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success));
        if (!success)
        {
            char infoLog[512];
            GLCall(glGetProgramInfoLog(shaderProgramID, 512, nullptr, infoLog));
            LOG_ERROR << "ERROR:SHADER_PROGRAM_LINKING_FAILED: " << infoLog << std::endl;
            return nullptr;
        }

        GLCall(glDeleteShader(vertexShader));
        GLCall(glDeleteShader(fragmentShader));

        return std::make_shared<ShaderProgram>(shaderProgramID);
    }

    //std::shared_ptr<QOpenGLShaderProgram> GraphicsAPI::CreateShaderProgram(const std::string& strShaderName)
    //{
    //    std::shared_ptr<QOpenGLShaderProgram> pShader = std::make_shared<QOpenGLShaderProgram>();

    //    SINGLETON_PTR(PathAppender)->SetSourcePath(":/Shader");
    //    std::string strVertexShaderPath = SINGLETON_PTR(PathAppender)->AppendChildPath(strShaderName + ".vert").GetPath();
    //    
    //    SINGLETON_PTR(PathAppender)->SetSourcePath(":/Shader");
    //    std::string strFragmentShaderPath = SINGLETON_PTR(PathAppender)->AppendChildPath(strShaderName + ".frag").GetPath();

    //    bool result = true;
    //    result = pShader->addShaderFromSourceFile(QOpenGLShader::Vertex, QString::fromStdString(strVertexShaderPath));
    //    if (!result) {
    //        qDebug() << pShader->log();
    //    }
    //    result = pShader->addShaderFromSourceFile(QOpenGLShader::Fragment, QString::fromStdString(strFragmentShaderPath));
    //    if (!result) {
    //        qDebug() << pShader->log();
    //    }
    //    result = pShader->link();
    //    if (!result) {
    //        qDebug() << pShader->log();
    //    }

    //    return pShader;
    //}

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

    void GraphicsAPI::SetClearColor(float r, float g, float b, float a)
    {
        OPENGLFUNC->glClearColor(r, g, b, a);
    }

    void GraphicsAPI::ClearBuffers()
    {
        OPENGLFUNC->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void GraphicsAPI::BindShaderProgram(ShaderProgram* shaderProgram)
    {
        if (nullptr != shaderProgram)
        {
            shaderProgram->Bind();
        }
    }

    void GraphicsAPI::BindMaterial(Material* material)
    {
        if (nullptr != material)
        {
            material->Bind();
        }
    }

    void GraphicsAPI::BindMesh(Mesh* mesh)
    {
        if (nullptr != mesh)
        {
            mesh->Bind();
        }
    }

    void GraphicsAPI::DrawMesh(Mesh* mesh)
    {
        if (nullptr != mesh)
        {
            mesh->Draw();
        }
    }

}