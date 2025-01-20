#include "Utility.h"

#include <fstream>
#include <sstream>

std::optional<std::string> Utility::LoadTextFromFile(const std::string& fileName)
{
	std::ifstream fs(fileName);

	if (fs.is_open() == false)
	{
		// ¡÷ºÆ
		return {};
	}

	std::stringstream ss;
	ss << fs.rdbuf();

	return ss.str();
}
