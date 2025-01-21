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

	glClear(GL_COLOR_BUFFER_BIT);

	{
		static float accTime = 0.0f;
		static float frameTime = 0.016f;

		float value = ::sinf(accTime) * 0.5f + 0.5f;
		auto uniformLocation = glGetUniformLocation(_program->GetID(), "color");
		_program->UseProgram();
		glUniform4f(uniformLocation, value* value, 2.0f * value * (1.0f - value), (1.0f - value)* (1.0f - value), 1.0f);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		accTime += frameTime;
	}
}

void Context::ProcessInput(GLFWwindow* window)
{

}

void Context::RefrechViewPort(int width, int height)
{
	_width = width;
	_height = height;
	glViewport(0, 0, width, height);
}

void Context::MouseMove(double x, double y)
{

}

void Context::MouseButton(int button, int action, double x, double y)
{

}

bool Context::Initialize()
{
	{
		//char buffer[MAX_PATH];
		//GetCurrentDirectory(MAX_PATH, buffer);

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
	}

	{
		std::shared_ptr<Shader> vertexShader = Shader::CreateFromFile("../shader/shader.vs", GL_VERTEX_SHADER);
		std::shared_ptr<Shader> fragmentShader = Shader::CreateFromFile("../shader/shader.fs", GL_FRAGMENT_SHADER);
		if (vertexShader == nullptr || fragmentShader == nullptr)
			return false;

		_program = Program::Create({ fragmentShader, vertexShader });
		if (_program == nullptr)
			return false;

		//	unifrom -> shader에서 사용할 수 있는 global 값 설정
		//	color : fs uniform name
		//	auto uniformLocation = glGetUniformLocation(_program->GetID(), "color");
		//	_program->UseProgram();
		//	glUniform4f(uniformLocation, 1.0f, 1.0f, 0.0f, 1.0f);*/
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

		auto texUniformLocation = glGetUniformLocation(_program->GetID(), "tex");
		glUniform1i(texUniformLocation, 0);
		auto tex2UniformLocation = glGetUniformLocation(_program->GetID(), "tex2");
		glUniform1i(tex2UniformLocation, 1);
	}


	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);
	return true;
}
