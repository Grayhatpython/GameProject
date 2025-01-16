#include "pch.h"
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "GameServer.h"
#include "Room.h"
#include "DBBind.h"
#include "DBConnectionPool.h"
#include "Exception.h"

GameServer* gameServer;

//BOOL CtrlHandler(DWORD CtrlType)
//{
//	switch (CtrlType)
//	{
//		// Handle the CTRL+C signal.
//	case CTRL_C_EVENT:
//	case CTRL_CLOSE_EVENT: // CTRL+CLOSE: confirm! that the user wants to exit.
//	case CTRL_BREAK_EVENT:
//	case CTRL_LOGOFF_EVENT:
//	case CTRL_SHUTDOWN_EVENT:
//	default:
//		gameServer->Close();
//		delete gameServer;
//		gameServer = nullptr;
//		return FALSE; 
//	}
//}

/*
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(6610);
	std::cout.imbue(std::locale(""));
	std::wcout.imbue(std::locale(""));

	::SetUnhandledExceptionFilter(ExceptionFilter);

	//ASSERT_CRASH(GDBConnectionPool->Connect(1, L"Driver={ODBC Driver 17 for SQL Server};server=(localdb)\\MSSQLLocalDB;database=GameServerDb;trusted_connection=Yes;"));

	//	Exit Function Callback Register
	/*BOOL successd = SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE);
	ASSERT_CRASH(successd);


	{
		gameServer = new GameServer();

		ASSERT_CRASH(gameServer->Initialize());
		ASSERT_CRASH(gameServer->Start());
		gameServer->DoProcessJob();
	}

	delete gameServer;

	system("pause");

	return 0;
}

*/

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "GLFW/glfw3.h"

// 성능 데이터를 저장하는 구조체
struct Task {
    std::string name;
    float startTime;
    float duration;
};

struct ThreadInfo {
    std::string threadName;
    std::vector<Task> tasks;
};

// 전역 변수로 성능 데이터를 저장할 벡터
std::vector<ThreadInfo> profilerData;
std::mutex profilerMutex;

// 타이머 시작 시간 (프로그램 시작 시간)
auto appStartTime = std::chrono::high_resolution_clock::now();

// 성능 측정 함수
void StartProfiling(const std::string& threadName, const std::string& taskName, std::function<void()> func) {
    auto start = std::chrono::high_resolution_clock::now();

    // 실제 실행할 함수
    func();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float, std::milli> duration = end - start;

    // 시작 시간 계산
    std::chrono::duration<float> timeSinceStart = start - appStartTime;

    // 데이터를 ThreadInfo에 추가
    std::lock_guard<std::mutex> lock(profilerMutex);
    bool threadExists = false;
    for (auto& threadInfo : profilerData) {
        if (threadInfo.threadName == threadName) {
            threadInfo.tasks.push_back({ taskName, timeSinceStart.count(), duration.count() });
            threadExists = true;
            break;
        }
    }

    if (!threadExists) {
        profilerData.push_back({ threadName, {{taskName, timeSinceStart.count(), duration.count()}} });
    }
}

// 프로파일러 UI를 그리는 함수
void RenderProfilerWindow() {
    ImGui::Begin("Profiler");

    //if (ImPlot::BeginPlot("Timeline", "Time (ms)", "Threads", ImVec2(-1, 400), ImPlotFlags_Crosshairs)) {
    //    // 타임라인의 축 범위 설정
    //    ImPlot::SetupAxesLimits(0, 1000, 0, profilerData.size() + 1, ImGuiCond_Always);

    //    int threadIdx = 1;
    //    for (const auto& threadInfo : profilerData) {
    //        for (const auto& task : threadInfo.tasks) {
    //            float taskStart = task.startTime;
    //            float taskEnd = task.startTime + task.duration;
    //            ImPlot::PlotShaded(threadInfo.threadName.c_str(), &taskStart, &taskEnd, 1, threadIdx);
    //        }
    //        threadIdx++;
    //    }
    //    ImPlot::EndPlot();
    //}

    // 스레드별 작업 목록 및 실행 시간 표시
    for (const auto& threadInfo : profilerData) {
        ImGui::Text("Thread: %s", threadInfo.threadName.c_str());
        for (const auto& task : threadInfo.tasks) {
            ImGui::Text("  Task: %s - %.2f ms", task.name.c_str(), task.duration);
        }
    }

    ImGui::End();
}

int main() {
    // GLFW 초기화
    if (!glfwInit()) return -1;

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Advanced Profiler Example", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    // ImGui 및 OpenGL 초기화
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    ImGui::StyleColorsDark();

    // 메인 루프
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // 새 프레임 시작
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 샘플로 여러 스레드를 프로파일링
        profilerData.clear();  // 이전 프레임의 데이터를 지우고 새로 측정
        std::thread t1([]() {
            StartProfiling("Thread 1", "Task A", []() { std::this_thread::sleep_for(std::chrono::milliseconds(16)); });
        StartProfiling("Thread 1", "Task B", []() { std::this_thread::sleep_for(std::chrono::milliseconds(8)); });
            });

        std::thread t2([]() {
            StartProfiling("Thread 2", "Task C", []() { std::this_thread::sleep_for(std::chrono::milliseconds(12)); });
        StartProfiling("Thread 2", "Task D", []() { std::this_thread::sleep_for(std::chrono::milliseconds(25)); });
            });

        t1.join();
        t2.join();

        // 프로파일러 UI 그리기
        RenderProfilerWindow();

        // ImGui 렌더링
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // 화면 갱신
        glfwSwapBuffers(window);
    }

    // 종료
    ImPlot::DestroyContext();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}



















/*
{
	//	DB Test
	ASSERT_CRASH(GDBConnectionPool->Connect(1, L"Driver={ODBC Driver 17 for SQL Server};server=(localdb)\\MSSQLLocalDB;database=GameServerDb;trusted_connection=Yes;"));

	//	Create Table
	{
		auto query = L"									\
			DROP TABLE IF EXISTS [dbo].[Gold];			\
			CREATE TABLE [dbo].[Gold]					\
			(											\
				[id] INT NOT NULL PRIMARY KEY IDENTITY, \
				[gold] INT NULL,						\
				[name] NVARCHAR(50) NULL,				\
				[createDate] DATETIME NULL				\
			);";

		auto dbConntion = GDBConnectionPool->Pop();
		ASSERT_CRASH(dbConntion->Execute(query));
		GDBConnectionPool->Push(dbConntion);
	}

	{
		for (int32 i = 0; i < 3; i++)
		{
			auto dbConntion = GDBConnectionPool->Pop();
			DBBind<3, 0> dbBind(*dbConntion, L"INSERT INTO [dbo].[Gold]([gold], [name], [createDate]) VALUES(?, ?, ?)");

			int32 gold = 100;
			dbBind.BindParam(0, gold);
			WCHAR name[100] = L"홍길동";
			dbBind.BindParam(1, name);
			TIMESTAMP_STRUCT ts = { 2024,5,18 };
			dbBind.BindParam(2, ts);
			ASSERT_CRASH(dbBind.Execute());

			GDBConnectionPool->Push(dbConntion);
		}
	}

	{
		auto dbConntion = GDBConnectionPool->Pop();
		DBBind<1, 4> dbBind(*dbConntion, L"SELECT id, gold, name, createDate FROM [dbo].[gold] WHERE gold = (?)");
		int32 gold = 100;
		dbBind.BindParam(0, gold);

		int32 outId = 0;
		int32 outGold = 0;
		WCHAR outName[100] = { 0, };
		TIMESTAMP_STRUCT outTs = { 0, };
		dbBind.BindCol(0, OUT outId);
		dbBind.BindCol(1, OUT outGold);
		dbBind.BindCol(2, OUT outName);
		dbBind.BindCol(3, OUT outTs);

		ASSERT_CRASH(dbBind.Execute());

		wcout.imbue(std::locale("kor"));

		while (dbBind.Fetch())
		{
			wcout << "id : " << outId << " Gold : " << outGold << " Name : " << outName << endl;
			wcout << "Date : " << outTs.year << "/" << outTs.month << "/" << outTs.day << endl;
		}

		GDBConnectionPool->Push(dbConntion);
	}
}


GRoom->PushReserveJob(1000, []()
	{
		cout << 1000 << endl;
	}
);
GRoom->PushReserveJob(2000, []() {cout << 2000 << endl; });
GRoom->PushReserveJob(3000, []() {cout << 3000 << endl; });
*/