
message(STATUS "Dependency.cmake: Start")

include(ExternalProject)

# PROJECT_BINARY_DIR - > build Folder
# DEP_INCLUDE_DIR -> build/external/include
# DEP_LIB_DIR -> build/external/lib
set(DEP_EXTERNAL_DIR ${PROJECT_BINARY_DIR}/external)
set(DEP_INCLUDE_DIR ${DEP_EXTERNAL_DIR}/include)
set(DEP_LIB_DIR ${DEP_EXTERNAL_DIR}/lib)

message(STATUS "Before adding spdlog")
# spdlog: fast logger library
ExternalProject_Add(
    dep-spdlog
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    GIT_TAG "v1.x"
    # GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEP_EXTERNAL_DIR}
    TEST_COMMAND ""
    TIMEOUT 30
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
    LOG_INSTALL ON
    LOG_OUTPUT_ON_FAILURE ON
)
message(STATUS "After adding spdlog")

# Dependency 리스트 및 라이브러리 파일 리스트 추가
set(DEP_LIST ${DEP_LIST} dep-spdlog)
set(DEP_LIBS ${DEP_LIBS} spdlog$<$<CONFIG:Debug>:d>)

# glfw
ExternalProject_Add(
    dep-glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw.git"
    GIT_TAG "3.3.4"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND "" 
    TEST_COMMAND ""
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${DEP_EXTERNAL_DIR}
        #   GLFW CMakeLists.txt -> option
        -DGLFW_BUILD_EXAMPLES=OFF   # 예제 x
        -DGLFW_BUILD_TESTS=OFF      # 테스트 x
        -DGLFW_BUILD_DOCS=OFF       # 문서 x
    )

set(DEP_LIST ${DEP_LIST} dep-glfw)
set(DEP_LIBS ${DEP_LIBS} glfw3)

# glad
ExternalProject_Add(
    dep-glad
    GIT_REPOSITORY "https://github.com/Dav1dde/glad"
    GIT_TAG "v0.1.36"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    TEST_COMMAND ""
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${DEP_EXTERNAL_DIR}
        -DGLAD_INSTALL=ON
    )

set(DEP_LIST ${DEP_LIST} dep-glad)
set(DEP_LIBS ${DEP_LIBS} glad)

# stb
ExternalProject_Add(
    dep-stb
    GIT_REPOSITORY "https://github.com/nothings/stb"
    GIT_TAG "master"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    TEST_COMMAND ""

    # Only Need Header file -> No Build 
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""

    # ${CMAKE_COMMAND} -E copy -> cmake -E copy
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy
        # Clone Space
        ${PROJECT_BINARY_DIR}/dep-stb-prefix/src/dep-stb/stb_image.h
        # Copy Space
        ${DEP_EXTERNAL_DIR}/include/stb/stb_image.h
    )

set(DEP_LIST ${DEP_LIST} dep-stb)

# glm
ExternalProject_Add(
  dep-glm
  GIT_REPOSITORY "https://github.com/g-truc/glm"
  GIT_TAG "0.9.9.8"
  GIT_SHALLOW 1
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  TEST_COMMAND ""
  # glm Directory Copy include/glm
  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_BINARY_DIR}/dep-glm-prefix/src/dep-glm/glm
    ${DEP_EXTERNAL_DIR}/include/glm
  )

set(DEP_LIST ${DEP_LIST} dep-glm)

# assimp
ExternalProject_Add(
  dep-assimp
  GIT_REPOSITORY "https://github.com/assimp/assimp"
  GIT_TAG "v5.0.1"
  GIT_SHALLOW 1
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=${DEP_EXTERNAL_DIR}
      -DBUILD_SHARED_LIBS=OFF
      -DASSIMP_BUILD_ASSIMP_TOOLS=OFF
      -DASSIMP_BUILD_TESTS=OFF
      -DASSIMP_INJECT_DEBUG_POSTFIX=OFF
      -DASSIMP_BUILD_ZLIB=ON
  TEST_COMMAND ""
  )
set(DEP_LIST ${DEP_LIST} dep-assimp)
set(DEP_LIBS ${DEP_LIBS} assimp-vc143-mt$<$<CONFIG:Debug>:d>
  zlibstatic$<$<CONFIG:Debug>:d>
  IrrXML$<$<CONFIG:Debug>:d>
  )

# imgui
# target name 
# No Update So Only Cpp file 
# ImGui 추가
add_library(imgui
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui.cpp
    imgui/imgui_impl_glfw.cpp
    imgui/imgui_impl_opengl3.cpp
)

# Include 디렉토리 설정
target_include_directories(imgui PRIVATE ${DEP_INCLUDE_DIR})
list(APPEND DEP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

# Dependency 리스트 추가
list(APPEND DEP_LIST imgui)
list(APPEND DEP_LIBS imgui)

# ImPlot 추가
add_library(implot
    implot/implot_items.cpp
    implot/implot.cpp
)

# Include 디렉토리 설정
list(APPEND DEP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/implot)
target_include_directories(implot PRIVATE ${DEP_INCLUDE_DIR})

# Dependency 설정
list(APPEND DEP_LIST implot)
list(APPEND DEP_LIBS implot)

# implot이 imgui에 의존하도록 설정
add_dependencies(implot imgui)
target_link_libraries(implot PRIVATE imgui)

# DEP_LIST -> dep-spdlog dep-glfw dep-glad dep-stb dep-glm imgui implot
# DEP_LIBS -> spdlogd(Debug) glfw3 glad imgui implot

#[[
${CMAKE_CURRENT_SOURCE_DIR}: 현재 CMakeLists.txt 파일이 있는 디렉토리.
${CMAKE_CURRENT_BINARY_DIR}: 현재 디렉토리에 빌드된 파일이 생성되는 디렉토리.
${CMAKE_SOURCE_DIR}: 프로젝트의 최상위 디렉토리.
${CMAKE_BINARY_DIR}: 프로젝트의 최상위 빌드 디렉토리.
]]

#[[
# protobuf
# 클라이언트 작업 후에 서버랑 연동 시에 수정 필요...
set(PROTOBUF_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protobuf)
set(PROTOC_BIN_DIR ${PROTOBUF_INCLUDE_DIR}/protoc)
set(PROTOBUF_LIB_DIR ${PROTOBUF_INCLUDE_DIR}/lib)
set(PROTOBUF_LIBRARIES ${PROTOBUF_LIB_DIR}/libprotobuf$<$<CONFIG:Debug>:d>.lib)

set(PROTO_GEN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PROTO_FILES ${PROTOC_BIN_DIR}/Enum.proto)

add_custom_command(
    OUTPUT ${PROTO_GEN_DIR}/Enum.pb.cc ${PROTO_GEN_DIR}/Enum.pb.h
    COMMAND ${PROTOC_BIN_DIR}/protoc --proto_path=${PROTOC_BIN_DIR} --cpp_out=${PROTO_GEN_DIR} ${PROTO_FILES}
    DEPENDS ${PROTO_FILES}
    COMMENT "Proto file build"
    VERBATIM
    USES_TERMINAL
)
    
set(DEP_INCLUDE_DIR ${DEP_INCLUDE_DIR} ${PROTOBUF_INCLUDE_DIR})

set(DEP_LIB_DIR ${DEP_LIB_DIR} ${PROTOBUF_LIB_DIR})
set(DEP_LIBS ${DEP_LIBS} libprotobuf$<$<CONFIG:Debug>:d>)
]]

# protobuf
set(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(PROTOBUF_INCLUDE_DIR ${PARENT_DIR}/Libraries/Include)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(PROTOBUF_LIB_DIR ${PARENT_DIR}/Libraries/lib/Protobuf/Debug)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(PROTOBUF_LIB_DIR ${PARENT_DIR}/Libraries/lib/Protobuf/Release)
else()
    set(PROTOBUF_LIB_DIR ${PARENT_DIR}/Libraries/lib/Protobuf/$<CONFIG>)
endif()

set(DEP_INCLUDE_DIR ${DEP_INCLUDE_DIR} ${PROTOBUF_INCLUDE_DIR})
set(DEP_LIB_DIR ${DEP_LIB_DIR} ${PROTOBUF_LIB_DIR})
set(DEP_LIBS ${DEP_LIBS} libprotobuf$<$<CONFIG:Debug>:d>)

# server Core
set(SERVER_CORE_INCLUDE_DIR ${PARENT_DIR}/ServerCore)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SERVER_CORE_LIB_DIR ${PARENT_DIR}/Libraries/lib/ServerCore/Debug)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(SERVER_CORE_LIB_DIR ${PARENT_DIR}/Libraries/lib/ServerCore/Release)
else()
    set(SERVER_CORE_LIB_DIR ${PARENT_DIR}/Libraries/lib/ServerCore/$<CONFIG>)
endif()

set(DEP_INCLUDE_DIR ${DEP_INCLUDE_DIR} ${SERVER_CORE_INCLUDE_DIR})
set(DEP_LIB_DIR ${DEP_LIB_DIR} ${SERVER_CORE_LIB_DIR})
set(DEP_LIBS ${DEP_LIBS} ServerCore>)

message(STATUS "Dependency.cmake: End")