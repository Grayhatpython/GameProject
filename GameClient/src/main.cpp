#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include "Core.h"

#include "Enum.pb.h"

int main(int argc, char** argv)
{   
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    {
        std::unique_ptr<Core> core = std::make_unique<Core>();
        assert(core->Initialize());

        core->Update();
        
        google::protobuf::ShutdownProtobufLibrary();
    }

    //_CrtMemState state;
    //_CrtMemCheckpoint(&state); // 특정 시점의 메모리 상태 저장

    //// 메모리 할당 및 작업 수행
    //int* leak = new int[10];

    //// 해당 시점 이후의 메모리 상태를 출력
    //_CrtMemDumpAllObjectsSince(&state);
    //_CrtMemDumpAllObjectsSince(nullptr);

    int leak = _CrtDumpMemoryLeaks();

    //_ASSERT(_CrtCheckMemory());

    return 0;
}