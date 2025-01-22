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
			ImGui::DragFloat3("Camera pos", glm::value_ptr(_cameraPos), 0.01f);
			ImGui::DragFloat("Camera yaw", &_cameraYaw, 0.1f);
			ImGui::DragFloat("Camera pitch", &_cameraPitch, 0.1f, -89.0f, 89.0f);
			ImGui::Separator();

			if (ImGui::Button("Reset camera")) 
			{
				_cameraYaw = 0.0f;
				_cameraPitch = 0.0f;
				_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
			}
		}

		ImGui::End();
	}

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

		for (size_t i = 0; i < cubePositions.size(); i++) {
			auto& pos = cubePositions[i];
			auto model = glm::translate(glm::mat4(1.0f), pos);
			model = glm::rotate(model,glm::radians((float)glfwGetTime() * 120.0f + 20.0f * (float)i),glm::vec3(1.0f, 0.5f, 0.0f));
			auto transform = projection * view * model;
			_program->SetUniform("transform", transform);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		}
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
		//char buffer[MAX_PATH];
		//GetCurrentDirectory(MAX_PATH, buffer);

		/*
		//	x y z	: position
		//	r g b	: color
		//	s t		: texture coordinate
		float vertices[] = {
		  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,	// top right
		  0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,	// bottom right
		  -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
		  -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,	// top left
		};

		uint32_t indices[] = {
			0, 1, 3,
			1, 2, 3
		};


		// 정점이 3개, 각 정점의 위치, 위치에 대해 x/y/z 값, 각 좌표값마다 float(4byte) 크기, 정점 간의 간격이 12bytes
		// vertexBuffer object가 가진 정점에 대한 구조를 알려줘야 한다!

		// Vertex Arrray Object (VAO)
		// 정점 데이터의 구조를 알려주는 object

		_vertexLayout = VertexLayout::Create();

		_vertexBuffer = Buffer::Create(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(vertices));
		assert(_vertexBuffer);

		// vertex buffer layout
		// vertex shader -> layout을 사용하여 attrib index 지정
		// vertex shader out 변수들은 Rasterization 과정을 거쳐 픽셀단위로 보간되어 fragment shader의 in 변수들로 입력
		// vertex -> x | y | z | r | g | b | s | t
		// 0 attrib ( xyz, offset 0 )
		_vertexLayout->EnableVertexAttribArray(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, 0);
		// 1 attrib ( rgb, offset 12 )
		_vertexLayout->EnableVertexAttribArray(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 3);
		// 2 attrib ( st, offset 24 )
		_vertexLayout->EnableVertexAttribArray(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, sizeof(float) * 6);

		_indexBuffer = Buffer::Create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(indices));
		assert(_indexBuffer);

		*/

		//	x y z	: position
		//	s t		: texture coordinate
		
		//	Cube
		float vertices[] = {
		  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		   0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

		  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
		   0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		   0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		  -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

		  -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		  -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

		   0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		   0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		   0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

		  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		   0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		   0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		  -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

		  -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
		   0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
		   0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
		  -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
		};

		uint32_t indices[] = {
		   0,  2,  1,  2,  0,  3,
		   4,  5,  6,  6,  7,  4,
		   8,  9, 10, 10, 11,  8,
		  12, 14, 13, 14, 12, 15,
		  16, 17, 18, 18, 19, 16,
		  20, 22, 21, 22, 20, 23,
		};
	
		_vertexLayout = VertexLayout::Create();

		_vertexBuffer = Buffer::Create(GL_ARRAY_BUFFER, GL_STATIC_DRAW, vertices, sizeof(vertices));
		assert(_vertexBuffer);

		// 0 attrib ( xyz, offset 0 )
		_vertexLayout->EnableVertexAttribArray(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
		// 1 attrib ( st, offset 12 )
		_vertexLayout->EnableVertexAttribArray(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, sizeof(float) * 3);

		_indexBuffer = Buffer::Create(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW, indices, sizeof(indices));
		assert(_indexBuffer);
	}

	{
		std::shared_ptr<Shader> vertexShader = Shader::CreateFromFile("../shader/shader.vs", GL_VERTEX_SHADER);
		std::shared_ptr<Shader> fragmentShader = Shader::CreateFromFile("../shader/shader.fs", GL_FRAGMENT_SHADER);
		if (vertexShader == nullptr || fragmentShader == nullptr)
			return false;

		_program = Program::Create({ fragmentShader, vertexShader });
		if (_program == nullptr)
			return false;


	}

	{
		auto image = Image::Load("../image/container.jpg");
		assert(image);
		printf("%d , %d", image->GetWidth(), image->GetHeight());
		
		_texture = Texture::Create(image.get());

		auto image2 = Image::Load("../image/awesomeface.png");
		assert(image2);
		printf("%d , %d", image2->GetWidth(), image2->GetHeight());

		_texture2 = Texture::Create(image2.get());

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
