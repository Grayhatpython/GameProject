#pragma once

#include "Context.h"

struct ImGuiContext;
struct ImPlotContext;
class Core
{
public:
	Core();
	~Core();

public:
	bool Initialize();

private:
	void InitializeEventCallback();

public:
	void Update();

public:
	static void OnFramebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void OnCursorPosCallback(GLFWwindow* window, double x, double y);
	static void OnMouseButtonCallback(GLFWwindow* window, int button, int action, int modifier);
	static void OnCharCallback(GLFWwindow* window, unsigned int ch);
	static void OnScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
	GLFWwindow*					_window = nullptr;
	std::unique_ptr<Context>	_context;
	ImGuiContext*				_imguiContext = nullptr;
	ImPlotContext*				_implotContext = nullptr;
};