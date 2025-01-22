#include "Program.h"
#include "Shader.h"

std::unique_ptr<Program> Program::Create(const std::vector<std::shared_ptr<Shader>>& shaders)
{
	auto program = std::unique_ptr<Program>(new Program());

	if (program->IsProgramLink(shaders) == false)
		return nullptr;

	return std::move(program);
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

		// ¡÷ºÆ
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

void Program::SetUniform(const std::string& name, const glm::mat4& value) const
{
	auto uniformLocation = glGetUniformLocation(_program, name.c_str());
	glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(value));
}
