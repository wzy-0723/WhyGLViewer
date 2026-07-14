#include "OpenGLRenderer.h"
#include "glview.h"
#include "singleton.h"
#include "OpenGLFunc.h"

OpenGLRenderer::OpenGLRenderer()
    : m_VBO(QOpenGLBuffer::VertexBuffer),
    m_EBO(QOpenGLBuffer::IndexBuffer)
{
	//initializeOpenGLFunctions();
    InitTriangle();
}

OpenGLRenderer::~OpenGLRenderer()
{
    m_VAO.destroy();
    m_VBO.destroy();
    m_EBO.destroy();
}

void OpenGLRenderer::InitTriangle()
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

void OpenGLRenderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
    SINGLETON_PTR(OpenGLFunc)->glViewport(x, y, width, height);
}

void OpenGLRenderer::ClearColor(const float32_t& fR, const float32_t& fG, const float32_t& fB, const float32_t& fA)
{
	SINGLETON_PTR(OpenGLFunc)->glClearColor((GLfloat)fR, (GLfloat)fG, (GLfloat)fB, (GLfloat)fA);
	SINGLETON_PTR(OpenGLFunc)->glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderer::InitShader(const QShaderInfo& shaderInfo)
{
    bool result = true;
    result = m_shader.addShaderFromSourceFile(QOpenGLShader::Vertex, shaderInfo.strVertexShaderPath);
    if (!result) {
        qDebug() << m_shader.log();
    }
    result = m_shader.addShaderFromSourceFile(QOpenGLShader::Fragment, shaderInfo.strFragmentShaderPath);
    if (!result) {
        qDebug() << m_shader.log();
    }
    result = m_shader.link();
    if (!result) {
        qDebug() << m_shader.log();
    }
}

void OpenGLRenderer::Draw(const std::vector<float32_t>& arrVertices, const std::vector<uint32_t>& arrIndices)
{
    m_shader.bind();

    GLuint uShaderID = m_shader.programId();
    GLint uColorLoc = SINGLETON_PTR(OpenGLFunc)->glGetUniformLocation(uShaderID, "uColor");

    SINGLETON_PTR(OpenGLFunc)->glUniform4f(uColorLoc, 0.0f, 1.0f, 0.0f, 1.0f);

    SINGLETON_PTR(OpenGLFunc)->glBindVertexArray(m_uVao);
    SINGLETON_PTR(OpenGLFunc)->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    m_shader.release();
}



