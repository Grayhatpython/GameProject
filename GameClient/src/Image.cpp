#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

std::unique_ptr<Image> Image::Load(const std::string& filepath)
{
	auto image = std::unique_ptr<Image>(new Image());
	if (image->LoadFromFile(filepath) == false)
		return nullptr;

	return std::move(image);
}

std::unique_ptr<Image> Image::Create(int width, int height, int channelCount)
{
	auto image = std::unique_ptr<Image>(new Image());
	if (image->Allocate(width, height, channelCount) == false)
		return nullptr;

	return std::move(image);
}

//	단일 컬러 이미지
std::unique_ptr<Image> Image::CreateSingleColorImage(int width, int height, const glm::vec4& color)
{
	glm::vec4 clamped = glm::clamp(color * 255.0f, 0.0f, 255.0f);
	uint8_t rgba[4] = {
	  (uint8_t)clamped.r,
	  (uint8_t)clamped.g,
	  (uint8_t)clamped.b,
	  (uint8_t)clamped.a,
	};
	auto image = Create(width, height, 4);
	for (int i = 0; i < width * height; i++) {
		memcpy(image->_data + 4 * i, rgba, 4);
	}
	return std::move(image);
}

Image::~Image()
{
	if (_data)
	{
		::stbi_image_free(_data);
		_data = nullptr;
	}
}

bool Image::LoadFromFile(const std::string& filepath)
{
	//	opengl 이미지는 0~1 사이의 범위
	//	일반 이미지는 좌상단을 원점 
	//	opengl은 좌하단을 원점
	//	이미지 로딩시 상하를 반전시켜야 한다.
	::stbi_set_flip_vertically_on_load(true);
	_data = ::stbi_load(filepath.c_str(), &_width, &_height, &_channelCount, 0);
	
	/*if (_channelCount == 1)
	{
		::stbi_image_free(_data);
		_data = nullptr;
		_data = ::stbi_load(filepath.c_str(), &_width, &_height, &_channelCount, 3);
	}*/

	if (_data == nullptr)
		return false;

	return true;
}

bool Image::Allocate(int width, int height, int channelCount)
{
	_width = width;
	_height = height;
	_channelCount = channelCount;

	_data = reinterpret_cast<uint8_t*>(::malloc(_width * _height * _channelCount));

	return _data ? true : false;
}
