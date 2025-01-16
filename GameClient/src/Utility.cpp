#include "Utility.h"

#include <fstream>
#include <sstream>

std::optional<std::string> Utility::LoadTextFromFile(const std::string& fileName)
{
	std::ifstream fs(fileName);

	if (fs.is_open() == false)
	{
		SPDLOG_ERROR("Failed to open File : {}", fileName);
		return {};
	}

	std::stringstream ss;
	ss << fs.rdbuf();

	return ss.str();
}
