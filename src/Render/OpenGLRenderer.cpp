#include "OpenGLRenderer.h"
#include "glview.h"
#include "Singleton.h"
#include "OpenGLFunc.h"
#include "GraphicsAPI.h"


Renderer::Renderer()
{
	//initializeOpenGLFunctions();
    InitTriangle();
}

Renderer::~Renderer()
{

}

void Renderer::InitTriangle()
{
    std::vector<float> vertices =
    {
        0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f,  0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 0.0f
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,
        0, 2, 3
    };
    
    GLuint vbo;
    SINGLETON_PTR(OpenGLFunc)->glGenBuffers(1, &vbo);
    SINGLETON_PTR(OpenGLFunc)->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    SINGLETON_PTR(OpenGLFunc)->glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
    SINGLETON_PTR(OpenGLFunc)->glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint ebo;
    SINGLETON_PTR(OpenGLFunc)->glGenBuffers(1, &ebo);
    SINGLETON_PTR(OpenGLFunc)->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    SINGLETON_PTR(OpenGLFunc)->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
    SINGLETON_PTR(OpenGLFunc)->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    SINGLETON_PTR(OpenGLFunc)->glGenVertexArrays(1, &m_uVao);
    SINGLETON_PTR(OpenGLFunc)->glBindVertexArray(m_uVao);
    SINGLETON_PTR(OpenGLFunc)->glBindBuffer(GL_ARRAY_BUFFER, vbo);
    SINGLETON_PTR(OpenGLFunc)->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);


    //@todo 自动计算偏移位置    
    SINGLETON_PTR(OpenGLFunc)->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    SINGLETON_PTR(OpenGLFunc)->glEnableVertexAttribArray(0);
    // vertex color
    SINGLETON_PTR(OpenGLFunc)->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    SINGLETON_PTR(OpenGLFunc)->glEnableVertexAttribArray(1);

    SINGLETON_PTR(OpenGLFunc)->glBindBuffer(GL_ARRAY_BUFFER, 0);
    SINGLETON_PTR(OpenGLFunc)->glBindVertexArray(0);
}

void Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    SINGLETON_PTR(OpenGLFunc)->glViewport(x, y, width, height);
}

void Renderer::ClearColor(const float32_t& fR, const float32_t& fG, const float32_t& fB, const float32_t& fA)
{
	SINGLETON_PTR(OpenGLFunc)->glClearColor((GLfloat)fR, (GLfloat)fG, (GLfloat)fB, (GLfloat)fA);
	SINGLETON_PTR(OpenGLFunc)->glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::InitShader(const QShaderInfo& shaderInfo)
{       
    m_pShader = SINGLETON_PTR(why::GraphicsAPI)->CreateShaderProgram("1.model_loading");
}

void Renderer::Draw(const std::vector<float32_t>& arrVertices, const std::vector<uint32_t>& arrIndices)
{
    m_pShader->bind();

    GLuint uShaderID = m_pShader->programId();
    GLint uColorLoc = SINGLETON_PTR(OpenGLFunc)->glGetUniformLocation(uShaderID, "uColor");

    SINGLETON_PTR(OpenGLFunc)->glUniform4f(uColorLoc, 0.0f, 1.0f, 0.0f, 1.0f);

    SINGLETON_PTR(OpenGLFunc)->glBindVertexArray(m_uVao);
    SINGLETON_PTR(OpenGLFunc)->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    m_pShader->release();
}



