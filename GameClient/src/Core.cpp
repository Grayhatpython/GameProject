#include "Core.h"
#include "Context.h"
#include "SceneManager.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <implot.h>


Core::Core()
{

}

Core::~Core()
{
    ImGui_ImplOpenGL3_DestroyFontsTexture();
    ImGui_ImplOpenGL3_DestroyDeviceObjects();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(_imguiContext);
    ImPlot::DestroyContext(_implotContext);

    ::glfwTerminate();
}

bool Core::Initialize()
{
    if (InitializeGL() == false)
        return false;

    InitializeImGui();
    InitializeEventCallback();
    
    _context = std::make_unique<Context>(new Context());
    _context->Initialize();
   
    _ASSERT(SceneManager::GetInstance()->Initialize());
    SceneManager::GetInstance()->LoadScene(L"Dev");
    Timer::GetInstance()->Initialize();

	return true;
}

void Core::InitializeEventCallback()
{
    ::glfwSetWindowUserPointer(_window, _context.get());

    ::glfwSetFramebufferSizeCallback(_window, OnFramebufferSizeCallback);
    ::glfwSetKeyCallback(_window, OnKeyCallback);
    ::glfwSetCursorPosCallback(_window, OnCursorPosCallback);
    ::glfwSetMouseButtonCallback(_window, OnMouseButtonCallback);
    ::glfwSetCharCallback(_window, OnCharCallback);
    ::glfwSetScrollCallback(_window, OnScrollCallback);
}

bool Core::InitializeGL()
{
    if (::glfwInit() == GLFW_FALSE)
    {
        const char* errorMsg = nullptr;
        ::glfwGetError(&errorMsg);
        return false;
    }

    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    ::glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    ::glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //::glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    _window = ::glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (_window == nullptr)
    {
        ::glfwTerminate();
        return false;
    }

    ::glfwMakeContextCurrent(_window);

    if (!::gladLoadGLLoader((GLADloadproc)::glfwGetProcAddress))
    {
        ::glfwTerminate();
        return false;
    }

    auto version = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    return true;
}

void Core::InitializeImGui()
{
    IMGUI_CHECKVERSION();
    _imguiContext = ImGui::CreateContext();
    _implotContext = ImPlot::CreateContext();

    ImGui::SetCurrentContext(_imguiContext);
    ImPlot::SetCurrentContext(_implotContext);

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(_window, false);
    ImGui_ImplOpenGL3_Init();

    auto& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(1.0, 1.0, 1.0, 1.0);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(1.0, 1.0, 1.0, 1.0);

    ImGui_ImplOpenGL3_CreateFontsTexture();
    ImGui_ImplOpenGL3_CreateDeviceObjects();
}

void Core::Update()
{
    //glfwSwapInterval(0);

    while (!::glfwWindowShouldClose(_window))
    {
        Timer::GetInstance()->Update();

        //  Poll
        ::glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        _context->ProcessInput(_window);
        
        Render();
    }

    Clear();
}

//  Render
void Core::Render()
{
    _context->Render();
    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    ::glfwSwapBuffers(_window);

    RenderFPS();
}

void Core::RenderFPS()
{
    uint32_t fps = Timer::GetInstance()->GetFPS();

    std::string title = "Opengl FPS : " + std::to_string(fps);
    glfwSetWindowTitle(_window, title.c_str());
}

void Core::Clear()
{
    SceneManager::GetInstance()->Clear();

    ::glfwTerminate();
}

void Core::OnFramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    auto context = reinterpret_cast<Context*>(::glfwGetWindowUserPointer(window));
    context->RefreshViewPort(width, height);
}

void Core::OnKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    ::ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

    /*SPDLOG_INFO("Keyboard Event - [ key : {}, scancode : {}, action : {}, mods : {}{}{}", key, scancode,
        action == GLFW_PRESS ? "Pressed" : action == GLFW_RELEASE ? "Released" : action == GLFW_REPEAT ? "Repeat" : "Unknown",
        mods & GLFW_MOD_CONTROL ? "Ctrl" : "_", mods & GLFW_MOD_SHIFT ? "Shift" : "_", mods & GLFW_MOD_ALT ? "Alt" : "_");*/

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        ::glfwSetWindowShouldClose(window, true);
}

void Core::OnCursorPosCallback(GLFWwindow* window, double x, double y)
{
    //SPDLOG_INFO("Gl OnCursorPos : {} , {}", x, y);
    auto context = reinterpret_cast<Context*>(::glfwGetWindowUserPointer(window));
    context->MouseMove(x, y);
}

void Core::OnMouseButtonCallback(GLFWwindow* window, int button, int action, int modifier)
{
    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, modifier);

    auto context = reinterpret_cast<Context*>(::glfwGetWindowUserPointer(window));
    double x, y;
    ::glfwGetCursorPos(window, &x, &y);
    context->MouseButton(button, action, x, y);
}

//  Number
void Core::OnCharCallback(GLFWwindow* window, unsigned int ch)
{
    ::ImGui_ImplGlfw_CharCallback(window, ch);
}

void Core::OnScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    ::ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
}