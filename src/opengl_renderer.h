#pragma once
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include "standard.h"

// 以Qt的规范加载着色器
struct QShaderInfo
{
	QString strVertexShaderPath;
	QString strFragmentShaderPath;	
};

class OpenGLRenderer
{
public:
	OpenGLRenderer();
	~OpenGLRenderer();

	void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
	void ClearColor(const float32_t& fR, const float32_t& fG, const float32_t& fB, const float32_t& fA);

	void InitShader(const QShaderInfo& shaderInfo);

	void Draw(const std::vector<float32_t>& arrVertices,const std::vector<uint32_t>& arrIndices);

	void InitTriangle();

private:
	QOpenGLVertexArrayObject m_VAO;
	QOpenGLBuffer m_VBO, m_EBO;
	QOpenGLShaderProgram m_shader;

private:
	GLuint m_uVao;
};