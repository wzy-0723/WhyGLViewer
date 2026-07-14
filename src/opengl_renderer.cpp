#include "opengl_renderer.h"

OpenGLRenderer::OpenGLRenderer()
    : m_VBO(QOpenGLBuffer::VertexBuffer),
    m_EBO(QOpenGLBuffer::IndexBuffer)
{
	initializeOpenGLFunctions();
}

OpenGLRenderer::~OpenGLRenderer()
{
    m_VAO.destroy();
    m_VBO.destroy();
    m_EBO.destroy();
}

void OpenGLRenderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
{
	glViewport(x, y, width, height);
}

void OpenGLRenderer::ClearColor(const float32_t& fR, const float32_t& fG, const float32_t& fB, const float32_t& fA)
{
	glClearColor((GLfloat)fR, (GLfloat)fG, (GLfloat)fB, (GLfloat)fA);
	glClear(GL_COLOR_BUFFER_BIT);
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
    //m_model->Draw(m_lightShader);

    if (!m_VAO.isCreated())
        m_VAO.create();
    else
        m_VAO.release();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_VAO);

    if (!m_VBO.isCreated())
        m_VBO.create();
    else
        m_VBO.release();
    if (!m_EBO.isCreated())
        m_EBO.create();
    else
        m_EBO.release();

    m_VBO.bind();
    m_VBO.allocate(arrVertices.data(), arrVertices.size() * sizeof(float));

    m_EBO.bind();
    m_EBO.allocate(arrIndices.data(), arrIndices.size() * sizeof(unsigned int));
    

    //@todo 自动计算偏移位置
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

    

    m_shader.setUniformValue("objectColor", 1.f, 0.2f, 0.2f, 1.f);

    QOpenGLVertexArrayObject::Binder bind(&m_VAO);
    glDrawElements(GL_TRIANGLES, arrIndices.size(), GL_UNSIGNED_INT, 0);

    m_shader.release();
}



