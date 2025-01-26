#include "Program.h"
#include "Shader.h"

std::unique_ptr<Program> Program::Create(const std::vector<std::shared_ptr<Shader>>& shaders)
{
	auto program = std::unique_ptr<Program>(new Program());

	if (program->IsProgramLink(shaders) == false)
		return nullptr;

	return std::move(program);
}

std::unique_ptr<Program> Program::Create(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
	std::shared_ptr<Shader> vs = Shader::CreateFromFile(vertexShaderFilename,GL_VERTEX_SHADER);
	std::shared_ptr<Shader> fs = Shader::CreateFromFile(fragmentShaderFilename,GL_FRAGMENT_SHADER);
	if (vs == nullptr || fs == nullptr)
		return nullptr;

	return std::move(Create({ vs, fs }));
}

Program::~Program()
{
	if (_program)
	{
		glDeleteProgram(_program);
	}
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
