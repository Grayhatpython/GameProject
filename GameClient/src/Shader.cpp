#include "Shader.h"

bool Shader::IsLoadFromFile(const std::string& fileName, GLenum shaderType)
{
	auto shaderText = Utility::LoadTextFromFile(fileName);

	if (shaderText.has_value() == false)
		return false;

	auto& shaderCode = shaderText.value();
	const char* shaderCodePtr = shaderCode.c_str();

	auto shaderCodeLength = static_cast<int32_t>(shaderCode.length());

	_shader = glCreateShader(shaderType);
	glShaderSource(_shader, 1, &shaderCodePtr, &shaderCodeLength);
	glCompileShader(_shader);

	GLint successed = 0;
	glGetShaderiv(_shader, GL_COMPILE_STATUS, &successed);

	if (!successed)
	{
		constexpr int MAX_LOG_INFO_LENGTH = 1024;
		char infoLog[MAX_LOG_INFO_LENGTH];
		glGetShaderInfoLog(_shader, MAX_LOG_INFO_LENGTH, nullptr, infoLog);

		// 주석
		return false;
	}

	return true;
}

Shader::~Shader()
{
	if (_shader)
	{
		glDeleteShader(_shader);
	}
}

bool Shader::Initialize(const std::string& fileName, GLenum shaderType)
{
	if (IsLoadFromFile(fileName, shaderType) == false)
		return false;

	return true;
}
