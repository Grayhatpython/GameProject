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
