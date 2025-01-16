#pragma once

class Shader
{
private:
	Shader() = default;

	bool IsLoadFromFile(const std::string& fileName, GLenum shaderType);

public:
	~Shader();

	static std::unique_ptr<Shader> CreateFromFile(const std::string& fileName, GLenum shaderType);

public:
	uint32_t	GetID() const { return _shader; }

private:
	uint32_t	_shader{ 0 };
};