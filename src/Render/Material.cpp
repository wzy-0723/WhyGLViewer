#include "Material.h"
#include "OpenGLFunc.h"

namespace why
{
	void Material::SetShaderProgram(const std::shared_ptr<QOpenGLShaderProgram>& shaderProgram)
	{
		m_pShaderProgram.reset(new ShaderProgram(shaderProgram));
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
		if (!m_pShaderProgram)
		{
			return;
		}

		m_pShaderProgram->Bind();

		for (auto& param : m_floatParams)
		{
			m_pShaderProgram->SetUniform(param.first.c_str(), param.second);
		}

		for (auto& param : m_float2Params)
		{
			//auto loc = OPENGLFUNC->glGetUniformLocation(id, param.first.c_str());
			//OPENGLFUNC->glUniform2f(loc, param.second.first, param.second.second);
			m_pShaderProgram->SetUniform(param.first.c_str(), param.second.first, param.second.second);
		}
	}
}