#include "Context.h"
#include "Shader.h"

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

	glUseProgram(_program->GetID());
	glDrawArrays(GL_POINTS, 0, 1);
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
	//char buffer[MAX_PATH];
	//GetCurrentDirectory(MAX_PATH, buffer);

	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
	};

	glGenBuffers(1, &_vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 9, vertices, GL_STATIC_DRAW);

	{
		std::shared_ptr<Shader> vertexShader = Shader::CreateFromFile("../shader/simple.vs", GL_VERTEX_SHADER);
		std::shared_ptr<Shader> fragmentShader = Shader::CreateFromFile("../shader/simple.fs", GL_FRAGMENT_SHADER);
		if (vertexShader == nullptr || fragmentShader == nullptr)
			return false;

		SPDLOG_INFO("Vertex Shader Id : {}", vertexShader->GetID());
		SPDLOG_INFO("Fragment Shader Id : {}", fragmentShader->GetID());

		_program = Program::Create({ fragmentShader, vertexShader });
		if (_program == nullptr)
			return false;

		SPDLOG_INFO("Program Id : {}", _program->GetID());
	}

	glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

	uint32_t vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	return true;
}
