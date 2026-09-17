cmake_minimum_required(VERSION 3.20)

include(FetchContent)

# --- Packages with real CMake support ---
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

# --- FMOD: proprietary binary SDK, no package manager distribution ---
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")
find_package(FMOD REQUIRED)

# --- Vendored: GLAD (generated GL loader, no upstream CMake package) ---
add_library(glad STATIC
  ${CMAKE_SOURCE_DIR}/external/glad/src/glad.c
)

target_include_directories(glad PUBLIC
  ${CMAKE_SOURCE_DIR}/external/glad/include
)

# --- Vendored: Dear ImGui (no upstream CMake package) ---
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.90.9   # match whatever version you were on in VS
)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
  ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC
  ${imgui_SOURCE_DIR}
  ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL glad)
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLAD)

# --- Aggregate interface library, mirroring your lvulkan_libraries pattern ---
add_library(game_libraries INTERFACE)
target_link_libraries(game_libraries
  INTERFACE
    glfw
    OpenGL::GL
    glad
    imgui
    assimp::assimp
    FMOD::FMOD
    FMOD::FMODStudio
)
