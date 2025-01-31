#include "Program.h"
#include "Shader.h"

Program::~Program()
{
	if (_program)
	{
		glDeleteProgram(_program);
	}
}

bool Program::Initialize(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
	std::shared_ptr<Shader> vs = std::make_shared<Shader>();
	if (vs->Initialize(vertexShaderFilename, GL_VERTEX_SHADER) == false)
		return false;
	
	std::shared_ptr<Shader> fs = std::make_shared<Shader>();
	if (vs->Initialize(fragmentShaderFilename, GL_FRAGMENT_SHADER) == false)
		return false;

	if (IsProgramLink({vs, fs}) == false)
		return false;

	return true);
}

bool Program::IsProgramLink(const std::vector<std::shared_ptr<Shader>>& shaders)
{
	_program = glCreateProgram();

	for (const auto& shader : shaders)
		glAttachShader(_program, shader->GetID());
	glLinkProgram(_program);

	GLint successed = 0;
	glGetProgramiv(_program, GL_LINK_STATUS, &successed);
	if (!successed)
	{
		constexpr int MAX_LOG_INFO_LENGTH = 1024;
		char infoLog[MAX_LOG_INFO_LENGTH];
		glGetProgramInfoLog(_program, MAX_LOG_INFO_LENGTH, nullptr, infoLog);

		// 주석
		return false;
	}

	return true;
}

void Program::UseProgram() const
{
	glUseProgram(_program);
}

void Program::SetUniform(const std::string& name, int value) const
{
	auto uniformLocation = glGetUniformLocation(_program, name.c_str());
	glUniform1i(uniformLocation, value);
}

void Program::SetUniform(const std::string& name, float value) const {
	auto uniformLocation = glGetUniformLocation(_program, name.c_str());
	glUniform1f(uniformLocation, value);
}

void Program::SetUniform(const std::string& name, const glm::vec2& value) const {
	auto uniformLocation = glGetUniformLocation(_program, name.c_str());
	glUniform2fv(uniformLocation, 1, glm::value_ptr(value));
}

void Program::SetUniform(const std::string& name, const glm::vec3& value) const {
	auto uniformLocation = glGetUniformLocation(_program, name.c_str());
	glUniform3fv(uniformLocation, 1, glm::value_ptr(value));
}

void Program::SetUniform(const std::string& name,const glm::vec4& value) const {
	auto uniformLocation = glGetUniformLocation(_program, name.c_str());
	glUniform4fv(uniformLocation, 1, glm::value_ptr(value));
}

void Program::SetUniform(const std::string& name, const glm::mat4& value) const
{
	auto uniformLocation = glGetUniformLocation(_program, name.c_str());
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
}
