
include(ExternalProject)

# PROJECT_BINARY_DIR - > build Folder
# DEP_INCLUDE_DIR -> build/external/include
# DEP_LIB_DIR -> build/external/lib
set(DEP_EXTERNAL_DIR ${PROJECT_BINARY_DIR}/external)
set(DEP_INCLUDE_DIR ${DEP_EXTERNAL_DIR}/include)
set(DEP_LIB_DIR ${DEP_EXTERNAL_DIR}/lib)

# spdlog: fast logger library
ExternalProject_Add(
    dep-spdlog
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    GIT_TAG "v1.x"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    TEST_COMMAND ""
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${DEP_EXTERNAL_DIR}
)

# Dependency 리스트 및 라이브러리 파일 리스트 추가
set(DEP_LIST ${DEP_LIST} dep-spdlog)
# Config Debug -> spdlogd.lib ( build Time )
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

# imgui
# target name 
# No Update So Only Cpp file 
add_library(imgui
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui.cpp
    imgui/imgui_impl_glfw.cpp
    imgui/imgui_impl_opengl3.cpp
    )

target_include_directories(imgui PRIVATE ${DEP_INCLUDE_DIR})
add_dependencies(imgui ${DEP_LIST})

# CMAKE_CURRENT_SOURCE_DIR -> build/imgui/
set(DEP_INCLUDE_DIR ${DEP_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

set(DEP_LIST ${DEP_LIST} imgui)
set(DEP_LIBS ${DEP_LIBS} imgui)

# implot
add_library(implot
    implot/implot_items.cpp
    implot/implot.cpp
    )

set(DEP_INCLUDE_DIR ${DEP_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/implot)

set(DEP_LIST ${DEP_LIST} implot)
set(DEP_LIBS ${DEP_LIBS} implot)

target_include_directories(implot PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
add_dependencies(implot imgui)

target_link_libraries(implot PRIVATE imgui)

# protobuf
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

# DEP_LIST -> dep-spdlog dep-glfw dep-glad dep-stb dep-glm imgui implot
# DEP_LIBS -> spdlogd(Debug) glfw3 glad imgui implot