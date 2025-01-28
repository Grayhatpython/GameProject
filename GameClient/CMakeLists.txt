cmake_minimum_required(VERSION 3.14)

set(PROJECT_NAME GameClient)
set(CMAKE_CXX_STANDARD 17)

# 실행 파일은 빌드 디렉토리 안에 bin 폴더에 출력.
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(WINDOW_NAME "Opengl Project")
set(WINDOW_WIDTH 1280)
set(WINDOW_HEIGHT 720)

project(${PROJECT_NAME})

set(CMAKE_VERBOSE_MAKEFILE true)

add_executable(${PROJECT_NAME}   
    src/main.cpp
    src/Context.cpp             src/Context.h
    src/Core.cpp                src/Core.h
    src/Shader.cpp              src/Shader.h
    src/Utility.cpp             src/Utility.h
    src/Program.cpp             src/Program.h
    src/Buffer.cpp              src/Buffer.h
    src/VertexLayout.cpp        src/VertexLayout.h
    src/Image.cpp               src/Image.h
    src/Texture.cpp             src/Texture.h
    src/Mesh.cpp                src/Mesh.h
    src/Model.cpp               src/Model.h
    src/Timer.cpp               src/Timer.h
    src/Component.cpp           src/Component.h
    src/GameObject.cpp          src/GameObject.h
    src/Transform.cpp           src/Transform.h
    src/MonoBehaviour.cpp       src/MonoBehaviour.h
    src/MeshRenderer.cpp        src/MeshRenderer.h
    src/SceneManager.cpp        src/SceneManager.h
    src/Scene.cpp               src/Scene.h
    src/Camera.cpp              src/Camera.h
)

target_precompile_headers(${PROJECT_NAME} PUBLIC src/pch.h)

include(Dependency.cmake)

# PUBLIC -> 해당 target에 의존성을 가지는 또 다른 타겟
# Ex) target이 라이브러리이고, 이 라이브러리를 링크하는 어플리케이션에도 동일한 정의가 적용
# PRIVATE -> 해당 target에만 적용

# 우리 프로젝트에 include / lib 관련 옵션 추가
# DEP_INCLUDE_DIR -> build/include
# DEP_LIB_DIR -> build/lib
# DEP_LIBS -> spdlogd(Debug) glfw3
target_include_directories(${PROJECT_NAME} PUBLIC ${DEP_INCLUDE_DIR})
target_link_directories(${PROJECT_NAME} PUBLIC ${DEP_LIB_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS})

# target_compile_definitions() -> 특정 타겟에 대한 컴파일 정의 ( #define )
target_compile_definitions(${PROJECT_NAME} PUBLIC
    WINDOW_NAME="${WINDOW_NAME}"
    WINDOW_WIDTH=${WINDOW_WIDTH}
    WINDOW_HEIGHT=${WINDOW_HEIGHT}
)

# Dependency들이 먼저 build 될 수 있게 관계 설정
add_dependencies(${PROJECT_NAME} ${DEP_LIST})


# CMake Config -> Build
# cmake -Bbuild . -DCMAKE_BUILD_TYPE=Debug|Release
# cmake --build build