#pragma once

// Texture
// Opengl texture object 생성 및 바인딩
// wrapping, filtering option 설정
// 이미지 데이터를 GPU 메모리로 복사
// shader 프로그램이 바인딩 되었을 때 (glUseProgrom) 사용하고자 하는 texture를 unifrom 형태로 프로그램에 전달

//	texture 적용
//	vertex attribute에 texture coordinate 추가
//	texture를 읽어서 픽셀값을 결정하는 shader 작성

class Image;
class Texture 
{
public:
    ~Texture();
    void Bind() const;

public:
    void Initialize(const Image* iBmage);

public:
    void SetFilter(uint32_t minFilter, uint32_t magFilter) const;
    void SetWrap(uint32_t sWrap, uint32_t tWrap) const;

    const uint32_t GetID() const { return _texture; }

public:
    uint32_t _texture{ 0 };
};
