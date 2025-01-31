#pragma once

class Shader
{
public:
	Shader() = default;
	~Shader();

public:
	bool Initialize(const std::string& fileName, GLenum shaderType);

private:
	bool IsLoadFromFile(const std::string& fileName, GLenum shaderType);


public:
	uint32_t	GetID() const { return _shader; }

private:
	uint32_t	_shader{ 0 };
};