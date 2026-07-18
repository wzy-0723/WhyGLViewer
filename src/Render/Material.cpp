#include "Material.h"
#include "OpenGLFunc.h"
#include <nlohmann/json.hpp>
#include "Engine.h"
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
	std::shared_ptr<Material> Material::Load(const std::string& path)
	{
		auto contents = SINGLETON_PTR(Engine)->GetFileSystem().LoadAssetFileText(path);

		if (contents.empty())
		{
			return nullptr;
		}

		nlohmann::json json = nlohmann::json::parse(contents);
		std::shared_ptr<Material> result;

		if (json.contains("shader"))
		{
			auto shaderObj = json["shader"];
			std::string vertexPath = shaderObj.value("vertex", "");
			std::string fragmentPath = shaderObj.value("fragment", "");

			auto& fs = SINGLETON_PTR(Engine)->GetFileSystem();
			auto vertexSrc = fs.LoadAssetFileText(vertexPath);
			auto fragmentSrc = fs.LoadAssetFileText(fragmentPath);

			auto shaderProgram = SINGLETON_PTR(GraphicsAPI)->CreateShaderProgram(vertexSrc, fragmentSrc);
			if (!shaderProgram)
			{
				return nullptr;
			}

			result = std::make_shared<Material>();
			result->SetShaderProgram(shaderProgram);
		}

		if (json.contains("params"))
		{
			auto paramsObj = json["params"];

			// Floats
			if (paramsObj.contains("float"))
			{
				for (auto& p : paramsObj["float"])
				{
					std::string name = p.value("name", "");
					float value = p.value("value", 0.0f);
					result->SetParam(name, value);
				}
			}

			// Float2
			if (paramsObj.contains("float2"))
			{
				for (auto& p : paramsObj["float2"])
				{
					std::string name = p.value("name", "");
					float v0 = p.value("value0", 0.0f);
					float v1 = p.value("value1", 0.0f);
					result->SetParam(name, v0, v1);
				}
			}

			// Textures
			if (paramsObj.contains("textures"))
			{
				for (auto& p : paramsObj["textures"])
				{
					std::string name = p.value("name", "");
					std::string texPath = p.value("path", "");
					auto texture = Texture::Load(texPath);

					result->SetParam(name, texture);
				}
			}
		}

		return result;
	}
}