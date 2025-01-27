#pragma once

class Image 
{
public:
    bool            Create(int width, int height, int channelCount = 4);
    bool            CreateSingleColorImage(int width, int height, const glm::vec4& color);
    ~Image();

public:
    bool            LoadFromFile(const std::string& filepath);

private:
    bool            Allocate(int width, int height, int channelCount);

public:
    int             GetWidth() const { return _width; }
    int             GetHeight() const { return _height; }
    int             GetChannelCount() const { return _channelCount; }
    const uint8_t*  GetData() const { return _data; }

private:

    int         _width{ 0 };
    int         _height{ 0 };
    int         _channelCount{ 0 };
    uint8_t*    _data{ nullptr };
};
