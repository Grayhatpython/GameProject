#include "Texture.h"
#include "Image.h"

Texture::~Texture()
{
	if (_texture)
	{
		glDeleteTextures(1, &_texture);
	}
}

std::unique_ptr<Texture> Texture::Create(const Image* image)
{
	auto texture = std::unique_ptr<Texture>(new Texture());
	texture->Initialize(image);
	return std::move(texture);
}

void Texture::Bind() const
{
	//	binding 과정 : 앞으로 이용할 GL_TEXTURE_2D -> _texture 
	glBindTexture(GL_TEXTURE_2D, _texture);
}

void Texture::Initialize(const Image* image)
{
	//	가로/세로 크기가 2의 지수형태일 때 GPU가 가장 효율적으로 처리할 수 있다.

	//	opengl texture object 생성 -> textureID
	glGenTextures(1, &_texture);

	Bind();

	//	Mipmap 
	//	GL_LINEAR_MIPMAP_LINEAR
	//	작은 사이즈의 이미지를 미리 준비하는 기법
	//	가장 큰 이미지 -> 0 level
	//	가로/세로 크기 절반씩 줄인 이미지를 미리 계산하여 level 1씩 증가시키며 저장
	//	원본 이미지 저장을 위해 필요한 메모리보다 1/3 만큼 더 소모

	//	GL_NEAREST_MIPMAP_NEAREST
	//	Mipmap중에 가장 가까운 Mipmap을 선택해서 그 중에서 가까운 픽셀을 선택한다.
	//	GL_LINEAR_MIPMAP_LINEAR
	//	Mipmap중에 적합한 두 레벨의 Mipmap을 선택해서 그 사이의 값을 interpolation 한다.

	SetFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);

	GLenum format = GL_RGBA;
	switch (image->GetChannelCount())
	{
	case 1: 
		format = GL_RED; break;
	case 2:
		format = GL_RG; break;
	case 3:
		format = GL_RGB; break;
	default:
		break;
	}

	//	CPU Image -> GPU로 복사
	//	Target : GL_TEXTURE_2D -> _texture
	glTexImage2D(GL_TEXTURE_2D,
		//	GPU 쪽의 Texture 구조 기술
		//  0 level : 기본 이미지 사이즈
		//  GL_RGBA : texture의 픽셀 포맷
		//	0 : Border 사이즈
		0, GL_RGBA, image->GetWidth(), image->GetHeight(), 0,
		//	image의 texture 구조 기술
		//	format : CPU Memory 상에 존재하는 image 픽셀 포맷
		format, GL_UNSIGNED_BYTE, image->GetData());

	//	Mipmap 
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::SetFilter(uint32_t minFilter, uint32_t magFilter) const
{

	//	GL_TEXTURE_MIN_FILTER 이미지 축소 필터
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	//	GL_TEXTURE_MIN_FILTER 이미지 확대 필터
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture::SetWrap(uint32_t sWrap, uint32_t tWrap) const
{

	//	이미지 범위가 0~1 사이가 아닐 경우의 래핑
	//	S : Texture 좌표계에서 x축
	//	T : Texture 좌표계에서 y축
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);

}
