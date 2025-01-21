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
	//	binding ���� : ������ �̿��� GL_TEXTURE_2D -> _texture 
	glBindTexture(GL_TEXTURE_2D, _texture);
}

void Texture::Initialize(const Image* image)
{
	//	����/���� ũ�Ⱑ 2�� ���������� �� GPU�� ���� ȿ�������� ó���� �� �ִ�.

	//	opengl texture object ���� -> textureID
	glGenTextures(1, &_texture);

	Bind();

	//	Mipmap 
	//	GL_LINEAR_MIPMAP_LINEAR
	//	���� �������� �̹����� �̸� �غ��ϴ� ���
	//	���� ū �̹��� -> 0 level
	//	����/���� ũ�� ���ݾ� ���� �̹����� �̸� ����Ͽ� level 1�� ������Ű�� ����
	//	���� �̹��� ������ ���� �ʿ��� �޸𸮺��� 1/3 ��ŭ �� �Ҹ�

	//	GL_NEAREST_MIPMAP_NEAREST
	//	Mipmap�߿� ���� ����� Mipmap�� �����ؼ� �� �߿��� ����� �ȼ��� �����Ѵ�.
	//	GL_LINEAR_MIPMAP_LINEAR
	//	Mipmap�߿� ������ �� ������ Mipmap�� �����ؼ� �� ������ ���� interpolation �Ѵ�.

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

	//	CPU Image -> GPU�� ����
	//	Target : GL_TEXTURE_2D -> _texture
	glTexImage2D(GL_TEXTURE_2D,
		//	GPU ���� Texture ���� ���
		//  0 level : �⺻ �̹��� ������
		//  GL_RGBA : texture�� �ȼ� ����
		//	0 : Border ������
		0, GL_RGBA, image->GetWidth(), image->GetHeight(), 0,
		//	image�� texture ���� ���
		//	format : CPU Memory �� �����ϴ� image �ȼ� ����
		format, GL_UNSIGNED_BYTE, image->GetData());

	//	Mipmap 
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::SetFilter(uint32_t minFilter, uint32_t magFilter) const
{

	//	GL_TEXTURE_MIN_FILTER �̹��� ��� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	//	GL_TEXTURE_MIN_FILTER �̹��� Ȯ�� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}

void Texture::SetWrap(uint32_t sWrap, uint32_t tWrap) const
{

	//	�̹��� ������ 0~1 ���̰� �ƴ� ����� ����
	//	S : Texture ��ǥ�迡�� x��
	//	T : Texture ��ǥ�迡�� y��
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, sWrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tWrap);

}
