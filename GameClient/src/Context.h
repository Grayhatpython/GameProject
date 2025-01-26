#pragma once

#include "Program.h"
#include "Mesh.h"
#include "Model.h"
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
	//	light
	std::unique_ptr<Program>	_program2;

	glm::vec4 _clearColor{ glm::vec4(0.3f, 0.3f, 0.3f, 0.0f) };
	
	int _width{ WINDOW_WIDTH };
	int _height{ WINDOW_HEIGHT };

	std::unique_ptr<Mesh>	_box;
	//std::unique_ptr<Model>	_model;

	//	texture
	std::unique_ptr<Texture> _texture;
	std::unique_ptr<Texture> _texture2;

	// camera 
	glm::vec3 _cameraPos{ glm::vec3(0.0f, 2.5f, 8.0f) };
	glm::vec3 _cameraFront{ glm::vec3(0.0f, 0.0f, -1.0f) };
	glm::vec3 _cameraUp{ glm::vec3(0.0f, 1.0f, 0.0f) };

	float		_cameraPitch{ -20.0f };
	float		_cameraYaw{ 0.0f };

	bool		_cameraControl{ false };
	glm::vec2	_prevMousePos{ glm::vec2{0.0f} };

	// lighting
	// light parameter
	struct Light {
		glm::vec3	position{ glm::vec3(1.0f, 4.0f, 4.0f) };
		glm::vec3	direction{ glm::vec3(-1.0f, -1.0f, -1.0f) };
		glm::vec2	cutoff{ glm::vec2(120.0f, 5.0f) };
		float		distance{ 128.0f };

		glm::vec3	ambient{ glm::vec3(0.1f, 0.1f, 0.1f) };
		glm::vec3	diffuse{ glm::vec3(0.5f, 0.5f, 0.5f) };
		glm::vec3	specular{ glm::vec3(1.0f, 1.0f, 1.0f) };
	};
	Light	_light;
	bool	_flashLightMode{ false };

	//Material _material;
	std::shared_ptr<Material> _planeMaterial;
	std::shared_ptr<Material> _box1Material;
	std::shared_ptr<Material> _box2Material;

	// animation
	bool		_animation{ true };

	//	File Explorer
	std::filesystem::path	_currentPath;
	std::filesystem::path	_selectedEntry;
};