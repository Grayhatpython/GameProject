#pragma once

#include "Buffer.h"
#include "Program.h"
#include "VertexLayout.h"
#include "Texture.h"
#include <filesystem>

class Context
{
public:
	Context();

public:
	static std::unique_ptr<Context> Create();

	void Render();

	void ProcessInput(GLFWwindow* window);
	void RefreshViewPort(int width, int height);
	void MouseMove(double x, double y);
	void MouseButton(int button, int action, double x, double y);

private:
	bool Initialize();

private:
	std::unique_ptr<Program>	_program;
	glm::vec4 _clearColor{ glm::vec4(0.3f, 0.3f, 0.3f, 0.0f) };
	
	int _width{ WINDOW_WIDTH };
	int _height{ WINDOW_HEIGHT };

	//	Buffer
	std::unique_ptr<Buffer> _vertexBuffer;
	std::unique_ptr<Buffer> _indexBuffer;

	//	layout
	std::unique_ptr<VertexLayout> _vertexLayout;

	//	texture
	std::unique_ptr<Texture> _texture;
	std::unique_ptr<Texture> _texture2;

	// camera 
	glm::vec3 _cameraPos{ glm::vec3(0.0f, 0.0f, 3.0f) };
	glm::vec3 _cameraFront{ glm::vec3(0.0f, 0.0f, -1.0f) };
	glm::vec3 _cameraUp{ glm::vec3(0.0f, 1.0f, 0.0f) };

	float		_cameraPitch{ 0.0f };
	float		_cameraYaw{ 0.0f };

	bool		_cameraControl{ false };
	glm::vec2	_prevMousePos{ glm::vec2{0.0f} };

	//	File Explorer
	std::filesystem::path	_currentPath;
	std::filesystem::path	_selectedEntry;
};