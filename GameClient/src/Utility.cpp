#include "Utility.h"

#include <fstream>
#include <sstream>

std::optional<std::string> Utility::LoadTextFromFile(const std::string& fileName)
{
	std::ifstream fs(fileName);

	if (fs.is_open() == false)
	{
		// 주석
		return {};
	}

	std::stringstream ss;
	ss << fs.rdbuf();

	return ss.str();
}

//	kc, kl, kq
glm::vec3 Utility::GetAttenuationCoeff(float distance)
{
	const auto linear_coeff = glm::vec4(
		8.4523112e-05, 4.4712582e+00, -1.8516388e+00, 3.3955811e+01
	);
	const auto quad_coeff = glm::vec4(
		-7.6103583e-04, 9.0120201e+00, -1.1618500e+01, 1.0000464e+02
	);

	float kc = 1.0f;
	float d = 1.0f / distance;
	auto dvec = glm::vec4(1.0f, d, d * d, d * d * d);
	float kl = glm::dot(linear_coeff, dvec);
	float kq = glm::dot(quad_coeff, dvec);

	return glm::vec3(kc, glm::max(kl, 0.0f), glm::max(kq * kq, 0.0f));
}
