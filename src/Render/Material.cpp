#include "Material.h"
#include "OpenGLFunc.h"

namespace why
{
	void Material::SetShaderProgram(const std::shared_ptr<QOpenGLShaderProgram>& shaderProgram)
	{
		m_shaderProgram = shaderProgram;
	}

	void Material::SetParam(const std::string& name, float value)
	{
		m_floatParams[name] = value;
	}

	void Material::SetParam(const std::string& name, float v0, float v1)
	{
		m_float2Params[name] = { v0, v1 };
	}

	void Material::Bind()
	{
		if (!m_shaderProgram)
		{
			return;
		}

		m_shaderProgram->bind();

		for (auto& param : m_floatParams)
		{
			m_shaderProgram->setUniformValue(param.first.c_str(), param.second);
		}


		GLuint id =m_shaderProgram->programId();
		for (auto& param : m_float2Params)
		{
			auto loc = OPENGLFUNC->glGetUniformLocation(id, param.first.c_str());
			OPENGLFUNC->glUniform2f(loc, param.second.first, param.second.second);
		}
	}
}