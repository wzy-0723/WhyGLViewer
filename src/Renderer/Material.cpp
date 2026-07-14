#include "Material.h"


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
	}
}