#pragma once

class Utility
{
public:
	static std::optional<std::string> LoadTextFromFile(const std::string& fileName);
	static glm::vec3 GetAttenuationCoeff(float distance);
};