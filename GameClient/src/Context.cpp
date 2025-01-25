#include "Context.h"
#include "Shader.h"
#include "Image.h"
#include "Texture.h"

#include <imgui.h>
#include <implot.h>

Context::Context()
	: _currentPath(std::filesystem::current_path()), _selectedEntry(std::filesystem::path{})
{
}

std::unique_ptr<Context> Context::Create()
{
	auto context = std::unique_ptr<Context>(new Context());
	if (context->Initialize() == false)
		return nullptr;

	return std::move(context);
}

void Context::Render()
{
	/*
	constexpr static auto S_ImGuiWindowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
	constexpr static auto S_WindowSize = ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT);
	constexpr static auto S_WindowPos = ImVec2(0.f, 0.f);

	{
		//	File Explorer
		ImGui::SetNextWindowPos(S_WindowPos);
		ImGui::SetNextWindowSize(S_WindowSize);

		// ImGui 윈도우 시작
		ImGui::Begin("Window", nullptr, S_ImGuiWindowFlags);
		
		{
			//	DrawMenu
			if (ImGui::Button("Go Up"))
			{
				if (_currentPath.has_parent_path())
				{
					_currentPath = _currentPath.parent_path();
				}
			}

			ImGui::SameLine();
			ImGui::Text("Current Directory : %s", _currentPath.string().c_str());
			ImGui::Separator();
		}

		{
			//	DrawContent
			for (const auto& entry : std::filesystem::directory_iterator(_currentPath))
			{
				const auto isSelected = entry.path() == _selectedEntry;
				const auto isDirectory = entry.is_directory();
				const auto isFile = entry.is_regular_file();
				auto entryName = entry.path().filename().string();

				if (isDirectory)
					entryName = "[D] : " + entryName;
				else if(isFile)
					entryName = "[F] : " + entryName;

				//ImGui::Text("%s", entryName.c_str());

				if (ImGui::Selectable(entryName.c_str(), isSelected))
				{
					if (isDirectory)
						_currentPath /= entry.path().filename();

					_selectedEntry = entry.path();
				}
			}

			ImGui::Separator();
		}

		{
			//	DrawActions

			ImGui::Separator();
		}

		{
			static char filter[16] = { "\0" };
			//	DrawFilter
			ImGui::Text("Filter : ");
			ImGui::SameLine();
			ImGui::InputText("###inFilter", filter, sizeof(filter));

			auto filterFileCount = std::size_t{ 0 };

			if (std::strlen(filter) != 0)
			{
				for (const auto& entry : std::filesystem::directory_iterator(_currentPath))
				{
					if (std::filesystem::is_regular_file(entry) == false)
						continue;

					if (entry.path().extension().string() == filter)
						++filterFileCount;
				}

				ImGui::Text("Number of files : %u", filterFileCount);
			}
		}

		// ImGui 윈도우 끝내기
		ImGui::End();
	}
	*/

	{
		//	ImGui

		if (ImGui::Begin("Editor")) 
		{
			if (ImGui::CollapsingHeader("Background Color"), ImGuiTreeNodeFlags_DefaultOpen)
			{
				ImGui::ColorEdit4("color", glm::value_ptr(_clearColor));
				glClearColor(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a);
			}

			if (ImGui::CollapsingHeader("Camera"), ImGuiTreeNodeFlags_DefaultOpen)
			{

				ImGui::DragFloat3("camera pos", glm::value_ptr(_cameraPos), 0.01f);
				ImGui::DragFloat("camera yaw", &_cameraYaw, 0.1f);
				ImGui::DragFloat("camera pitch", &_cameraPitch, 0.1f, -89.0f, 89.0f);
				ImGui::Separator();

				if (ImGui::Button("Reset camera"))
				{
					_cameraYaw = 0.0f;
					_cameraPitch = 0.0f;
					_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
				}
			}

			if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::DragFloat3("position", glm::value_ptr(_light.position), 0.01f);
				ImGui::DragFloat3("direction", glm::value_ptr(_light.direction), 0.01f);
				ImGui::DragFloat("distance", &_light.distance, 0.5, 0.0f, 3000.0f);
				ImGui::DragFloat2("cutoff", glm::value_ptr(_light.cutoff), 0.5f, 0.0f, 180.0f);
				ImGui::ColorEdit3("ambient", glm::value_ptr(_light.ambient));
				ImGui::ColorEdit3("diffuse", glm::value_ptr(_light.diffuse));
				ImGui::ColorEdit3("specular", glm::value_ptr(_light.specular));
				ImGui::Checkbox("Flash light", &_flashLightMode);
			}

			if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
			{
				ImGui::DragFloat("m.shininess", &_material.shininess, 1.0f, 1.0f, 256.0f);
			}

			ImGui::Checkbox("Animation", &_animation);
		}

		ImGui::End();
	}


	//	Depth Buffer
	//	해당 픽셀의 깊이갑 (z축값)을 저장
	//	픽셀의 값을 업데이트 전에 현재 그리려는 픽셀의 z값과 깊이 버퍼에 저장된 해당 위치의 z값을 비교해서
	//	현재 그기려는 픽셀이 이전에 그려진 픽셀보다 뒤에 있을 경우 픽셀을 그리지 않음.
	//	1이 가장 뒤에 있고, 0이 가장 앞 (왼손 좌표계)

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	{
		//	pitch(x) 끄덕각
		//	yaw(y)	도리각
		//	roll(z) 갸웃?각

		_cameraFront = glm::rotate(glm::mat4(1.0f), glm::radians(_cameraYaw), glm::vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(glm::mat4(1.0f), glm::radians(_cameraPitch), glm::vec3(1.0f, 0.0f, 0.0f)) *
			glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	
		auto projection = glm::perspective(glm::radians(45.0f), (float)_width / (float)_height, 0.01f, 30.0f);

		/*
		float angle = glfwGetTime() * glm::pi<float>() * 0.5f;
		auto x = sinf(angle) * 10.0f;
		auto z = cosf(angle) * 10.0f;
		auto cameraPos = glm::vec3(x, 0.0f, z);
		auto cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		auto cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

		auto cameraZ = glm::normalize(cameraPos - cameraTarget);
		auto cameraX = glm::normalize(glm::cross(cameraUp, cameraZ));
		auto cameraY = glm::cross(cameraZ, cameraX);

		auto cameraMat = glm::mat4(
			glm::vec4(cameraX, 0.0f),
			glm::vec4(cameraY, 0.0f),
			glm::vec4(cameraZ, 0.0f),
			glm::vec4(cameraPos, 1.0f));

		auto view = glm::inverse(cameraMat);
		*/

		auto view = glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp);

		/*
		//	여러 개의 랜덤 회전하는 박스 렌더링
		std::vector<glm::vec3> cubePositions = {
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(2.0f, 5.0f, -15.0f),
			glm::vec3(-1.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(2.4f, -0.4f, -3.5f),
			glm::vec3(-1.7f, 3.0f, -7.5f),
			glm::vec3(1.3f, -2.0f, -2.5f),
			glm::vec3(1.5f, 2.0f, -2.5f),
			glm::vec3(1.5f, 0.2f, -1.5f),
			glm::vec3(-1.3f, 1.0f, -1.5f),
		};

		_program->UseProgram();
		_program->SetUniform("viewPos", _cameraPos);
		_program->SetUniform("light.position", _light.position);
		_program->SetUniform("light.direction", _light.direction);
		_program->SetUniform("light.cutoff", glm::vec2(
			cosf(glm::radians(_light.cutoff[0])),
			cosf(glm::radians(_light.cutoff[0] + _light.cutoff[1]))));
		_program->SetUniform("light.attenuation", Utility::GetAttenuationCoeff(_light.distance));
		_program->SetUniform("light.ambient", _light.ambient);
		_program->SetUniform("light.diffuse", _light.diffuse);
		_program->SetUniform("light.specular", _light.specular);
		_program->SetUniform("material.diffuse", 0);
		_program->SetUniform("material.specular", 1);
		_program->SetUniform("material.shininess", _material.shininess);

		glActiveTexture(GL_TEXTURE0);
		_material.diffuse->Bind();
		glActiveTexture(GL_TEXTURE1);
		_material.specular->Bind();

		for (size_t i = 0; i < cubePositions.size(); i++) {
			auto& pos = cubePositions[i];
			auto model = glm::translate(glm::mat4(1.0f), pos);
			model = glm::rotate(model,glm::radians((_animation ? (float)glfwGetTime() : 0.0f) * 120.0f + 20.0f * (float)i),glm::vec3(1.0f, 0.5f, 0.0f));
			auto transform = projection * view * model;
			_program->SetUniform("transform", transform);
			_program->SetUniform("modelTransform", model);
			_box->Render();
		}*/

		glm::vec3 lightPos = _light.position;
		glm::vec3 lightDir = _light.direction;
		if (_flashLightMode)
		{
			//	카메라의 위치와 방향이 곧 라이트랑 같다..
			lightPos = _cameraPos;
			lightDir = _cameraFront;
		}
		else
		{
			//	light
			auto lightModelTransform = glm::translate(glm::mat4(1.0), _light.position) * glm::scale(glm::mat4(1.0), glm::vec3(0.1f));

			_program2->UseProgram();
			_program2->SetUniform("color", glm::vec4(_light.ambient + _light.diffuse, 1.0f));
			_program2->SetUniform("transform", projection * view * lightModelTransform);

			_box->Render(_program2.get());
		}

		_program->UseProgram();
		_program->SetUniform("viewPos", _cameraPos);
		_program->SetUniform("light.position", lightPos);
		_program->SetUniform("light.direction", lightDir);
		_program->SetUniform("light.cutoff", glm::vec2(
			cosf(glm::radians(_light.cutoff[0])),
			cosf(glm::radians(_light.cutoff[0] + _light.cutoff[1]))));
		_program->SetUniform("light.attenuation", Utility::GetAttenuationCoeff(_light.distance));
		_program->SetUniform("light.ambient", _light.ambient);
		_program->SetUniform("light.diffuse", _light.diffuse);
		_program->SetUniform("light.specular", _light.specular);

		//	Texture 바인딩
		_program->SetUniform("material.diffuse", 0);
		_program->SetUniform("material.specular", 1);

		_program->SetUniform("material.shininess", _material.shininess);

	

		auto modelTransform = glm::mat4(1.0f);
		auto transform = projection * view * modelTransform;
		_program->SetUniform("transform", transform);
		_program->SetUniform("modelTransform", modelTransform);
		_model->Render(_program.get());
	}
}

void Context::ProcessInput(GLFWwindow* window)
{
	if (_cameraControl == false)
		return;

	const float cameraSpeed = 0.05f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		_cameraPos += cameraSpeed * _cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		_cameraPos -= cameraSpeed * _cameraFront;

	auto cameraRight = glm::normalize(glm::cross(_cameraUp, -_cameraFront));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		_cameraPos += cameraSpeed * cameraRight;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		_cameraPos -= cameraSpeed * cameraRight;

	auto cameraUp = glm::normalize(glm::cross(-_cameraFront, cameraRight));
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		_cameraPos += cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		_cameraPos -= cameraSpeed * cameraUp;
}

//	projection 행렬 : 창 크기 변경 적용
void Context::RefreshViewPort(int width, int height)
{
	_width = width;
	_height = height;

	glViewport(0, 0, width, height);
}

void Context::MouseMove(double x, double y)
{
	if (_cameraControl == false)
		return;

	auto pos = glm::vec2((float)x, (float)y);
	auto deltaPos = pos - _prevMousePos;

	const float cameraRotSpeed = 0.2f;
	_cameraYaw -= deltaPos.x * cameraRotSpeed;
	_cameraPitch -= deltaPos.y * cameraRotSpeed;

	if (_cameraYaw < 0.0f)   
		_cameraYaw += 360.0f;
	if (_cameraYaw > 360.0f) 
		_cameraYaw -= 360.0f;

	if (_cameraPitch > 89.0f)  
		_cameraPitch = 89.0f;
	if (_cameraPitch < -89.0f) 
		_cameraPitch = -89.0f;

	_prevMousePos = pos;
}

void Context::MouseButton(int button, int action, double x, double y)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (action == GLFW_PRESS) {
			// 마우스 조작 시작 시점에 현재 마우스 커서 위치 저장
			_prevMousePos = glm::vec2((float)x, (float)y);
			_cameraControl = true;
		}
		else if (action == GLFW_RELEASE) {
			_cameraControl = false;
		}
	}
}

bool Context::Initialize()
{
	{
		_box = Mesh::CreateBox();
		_model = Model::Load("../model/backpack.obj");
		assert(_model);
	}

	{
		_program = Program::Create("../shader/lighting.vs", "../shader/lighting.fs");
		if (_program == nullptr)
		  return false;
		_program2 = Program::Create("../shader/basic.vs", "../shader/basic.fs");
		if (_program2 == nullptr)
		  return false;
	}

	{
		auto image = Image::Load("../image/container.jpg");
		assert(image);
		printf("Image : %s , Width : %d , Height : %d\n", "container.jpg", image->GetWidth(), image->GetHeight());
		
		_texture = Texture::Create(image.get());

		auto image2 = Image::Load("../image/awesomeface.png");
		assert(image2);
		printf("Image2 : %s , Width : %d , Height : %d\n", "awesomeface.jpg", image2->GetWidth(), image2->GetHeight());

		_texture2 = Texture::Create(image2.get());

		//_material.diffuse = Texture::Create(Image::Load("../image/container2.png").get());
		//_material.specular = Texture::Create(Image::Load("../image/container2_specular.png").get());

		_material.diffuse = Texture::Create(Image::CreateSingleColorImage(4, 4, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)).get());
		_material.specular = Texture::Create(Image::CreateSingleColorImage(4, 4, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)).get());

		//	texture를 shader program에 제공하는 방법
		//	glActiveTexture(textureSlot)			->	현재 다루고자 하는 texture slot 선택
		//	glBindTexture(textureType, textureID)	->	현재 설정중인 texture slot에 texture object binding
		//	glGetUniformLocation()					->	shader 내의 sampler2D uniform handle을 얻어온다.
		//	glUniform1i()							->	sampler2D uniform에 texture slot index 입력
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _texture->GetID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, _texture2->GetID());

		_program->UseProgram();

		_program->SetUniform("tex",0);
		_program->SetUniform("tex2",1);
	}

	{	
		/*
		//	unifrom -> shader에서 사용할 수 있는 global 값 설정
		//	fs uniform name

		// x축으로 -55도 회전
		auto model = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		// 카메라는 원점으로부터 z축 방향으로 -3만큼 떨어짐
		auto view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
		// 종횡비 4:3, 세로화각 45도의 원근 투영
		auto projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH/ (float)WINDOW_HEIGHT, 0.01f, 10.0f);

		auto transform = projection * view * model;
		_program->SetUniform("transform", transform);
		*/
	}

	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	return true;
}
