#pragma once

class Shader;
class Program
{
public:
	static std::unique_ptr<Program> Create(const std::vector<std::shared_ptr<Shader>>& shaders);
	static std::unique_ptr<Program> Create(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	~Program();

private:
	Program() = default;
	bool IsProgramLink(const std::vector<std::shared_ptr<Shader>>& shaders);

public:
	void UseProgram() const;

public:
	void	SetUniform(const std::string& name, int value) const;
	void	SetUniform(const std::string& name, float value) const;
	void	SetUniform(const std::string& name, const glm::vec2& value) const;
	void	SetUniform(const std::string& name, const glm::vec3& value) const;
	void	SetUniform(const std::string& name, const glm::vec4& value) const;
	void	SetUniform(const std::string& name, const glm::mat4& value) const;

	uint32_t GetID() const { return _program; }

private:
	uint32_t	_program{ 0 };
};