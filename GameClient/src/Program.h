#pragma once

class Shader;
class Program
{
public:
	static std::unique_ptr<Program> Create(const std::vector<std::shared_ptr<Shader>>& shaders);
	~Program();

private:
	Program() = default;
	bool IsProgramLink(const std::vector<std::shared_ptr<Shader>>& shaders);

public:
	void UseProgram() const;

public:
	uint32_t GetID() const { return _program; }

private:
	uint32_t	_program{ 0 };
};