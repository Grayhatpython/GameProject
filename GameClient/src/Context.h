#pragma once

#include "Buffer.h"
#include "Program.h"
#include "VertexLayout.h"
#include <filesystem>

class Context
{
public:
	Context();

public:
	static std::unique_ptr<Context> Create();

	void Render();

	void ProcessInput(GLFWwindow* window);
	void RefrechViewPort(int width, int height);
	void MouseMove(double x, double y);
	void MouseButton(int button, int action, double x, double y);

private:
	bool Initialize();

private:
	std::unique_ptr<Program>	_program;
	glm::vec4 _clearColor{ glm::vec4(0.3f, 0.3f, 0.3f, 0.0f) };
	
	int _width{ WINDOW_WIDTH };
	int _height{ WINDOW_HEIGHT };

	
	std::unique_ptr<Buffer> _vertexBuffer;
	std::unique_ptr<Buffer> _indexBuffer;

	std::unique_ptr<VertexLayout> _vertexLayout;

	//	File Explorer
	std::filesystem::path	_currentPath;
	std::filesystem::path	_selectedEntry;
};