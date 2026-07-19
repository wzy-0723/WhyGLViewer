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

    const std::shared_ptr<ShaderProgram>& GraphicsAPI::GetDefaultShaderProgram()
    {
        if (!m_defaultShaderProgram)
        {
            std::string vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 position;
            layout (location = 1) in vec3 color;
            layout (location = 2) in vec2 uv;
            layout (location = 3) in vec3 normal;
        
            out vec2 vUV;
            out vec3 vNormal;
            out vec3 vFragPos;
        
            uniform mat4 uModel;
            uniform mat4 uView;
            uniform mat4 uProjection;
        
            void main()
            {
                vUV = uv;
                vNormal = normalize(transpose(inverse(mat3(uModel))) * normal);
                vFragPos = vec3(uModel * vec4(position, 1.0));
                gl_Position = uProjection * uView * uModel * vec4(position, 1.0);
            }
            )";

            std::string fragmentShaderSource = R"(
            #version 330 core

            struct Light
            {
                vec3 color;
                vec3 position;
            };

            uniform Light uLight;
            uniform vec3 uCameraPos;

            out vec4 FragColor;

            in vec2 vUV;
            in vec3 vNormal;
            in vec3 vFragPos;

            uniform sampler2D baseColorTexture;

            void main()
            {
                vec3 norm = normalize(vNormal);

                // diffuse
                vec3 lightDir = normalize(uLight.position - vFragPos);
                float diff = max(dot(norm, lightDir), 0.0);
                vec3 diffuse = diff * uLight.color;

                // specular
                vec3 viewDir = normalize(uCameraPos - vFragPos);
                vec3 redlectDir = reflect(-lightDir, norm);                 //reflect(入射向量I, 法向量N)，返回反射光线方向。                
                //spec：高光强度系数 0~1。
                //点积，即视线在反射上的投影。
                //幂次就是高光粗糙度 / 反光集中度，指数 32：高光区域很小、光斑锐利，像金属、光滑塑料
                float spec = pow(max(dot(viewDir, redlectDir), 0.0), 32.0); 
                //强度系数
                float specularStrength = 0.5;               
                vec3 specular = specularStrength * spec * uLight.color;

                vec3 result = diffuse + specular;


                vec4 texColor = texture(baseColorTexture, vUV);

                FragColor = texColor * vec4(result, 1.0);
            }
            )";

            m_defaultShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
        }

        return m_defaultShaderProgram;
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