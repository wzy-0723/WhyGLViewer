#include "Material.h"
#include "OpenGLFunc.h"

namespace why
{
	void Material::SetShaderProgram(const std::shared_ptr<ShaderProgram>& shaderProgram)
	{
		m_pShaderProgram = shaderProgram;
	}

	void Material::SetParam(const std::string& name, float value)
	{
		m_floatParams[name] = value;
	}

	void Material::SetParam(const std::string& name, float v0, float v1)
	{
		m_float2Params[name] = { v0, v1 };
	}

	void Material::SetParam(const std::string& name, const std::shared_ptr<Texture>& texture)
	{
		m_textures[name] = texture;
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
			m_pShaderProgram->SetUniform(param.first.c_str(), param.second.first, param.second.second);
		}

		for (auto& param : m_textures)
		{
			m_pShaderProgram->SetTexture(param.first, param.second.get());
		}
	}
}