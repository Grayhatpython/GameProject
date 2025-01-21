#pragma once

// Texture
// Opengl texture object ���� �� ���ε�
// wrapping, filtering option ����
// �̹��� �����͸� GPU �޸𸮷� ����
// shader ���α׷��� ���ε� �Ǿ��� �� (glUseProgrom) ����ϰ��� �ϴ� texture�� unifrom ���·� ���α׷��� ����

//	texture ����
//	vertex attribute�� texture coordinate �߰�
//	texture�� �о �ȼ����� �����ϴ� shader �ۼ�

class Image;
class Texture 
{
private:
    Texture() = default;

public:
    ~Texture();
    static std::unique_ptr<Texture> Create(const Image* image);
    void Bind() const;

private:
    void Initialize(const Image* image);

public:
    void SetFilter(uint32_t minFilter, uint32_t magFilter) const;
    void SetWrap(uint32_t sWrap, uint32_t tWrap) const;

    const uint32_t GetID() const { return _texture; }

public:
    uint32_t _texture{ 0 };
};
