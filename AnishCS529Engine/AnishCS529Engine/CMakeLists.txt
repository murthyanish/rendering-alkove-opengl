cmake_minimum_required(VERSION 4.3)

project(hawthorne_engine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS ON)

### Set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory
# automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

### Importing libraries.
# I'm placing all the libraries in this cmake to make things easier. I'm
# exporting the target lvulkan_libraries so I can quickly use them in the
# project.
include(CMake/libraries.cmake)

add_executable(hawthorne_engine)

target_precompile_headers(hawthorne_engine
  PRIVATE
    precompiled.h
)

target_include_directories(hawthorne_engine PUBLIC
  ${CMAKE_SOURCE_DIR}/src
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/external/glad/include
  ${CMAKE_SOURCE_DIR}/include/stb
)

target_sources(hawthorne_engine
  PRIVATE
    src/AABB.cpp
    src/AmbientOcclusionPass.cpp
    src/Animate.cpp
    src/AttachedCamera.cpp
    src/Audio.cpp
    src/Camera.cpp
    src/CollisionGenerator.cpp
    src/CollisionListener.cpp
    src/Component.cpp
    src/DeathPlane.cpp
    src/Door.cpp
    src/EngineMath.cpp
    src/EventManager.cpp
    src/FBO.cpp
    src/FPCDemo.cpp
    src/FirstPersonControllerComponent.cpp
    src/FreeCamera.cpp
    src/GBufferPrepass.cpp
    src/GameObject.cpp
    src/GameWindow.cpp
    src/GeometryBuffer.cpp
    src/InGameKey.cpp
    src/Input.cpp
    src/KeyList.cpp
    src/LMMain.cpp
    src/LevelManager.cpp
    src/LightingPass.cpp
    src/LocalLightsPass.cpp
    src/MapGraphics.cpp
    src/MapLoader.cpp
    src/Map_Advanced.cpp
    src/Map_Intermediate.cpp
    src/Map_Intermediate2.cpp
    src/Map_Intermediate3.cpp
    src/Map_Menu.cpp
    src/Map_Tutorial.cpp
    src/Map_Tutorial2.cpp
    src/Map_Victory.cpp
    src/Material.cpp
    src/Matrix4.cpp
    src/Mesh.cpp
    src/MomentShadowMapPass.cpp
    src/Node.cpp
    src/OBB.cpp
    src/PauseMenu.cpp
    src/PhysicsBody.cpp
    src/PhysicsManager.cpp
    src/Quaternion.cpp
    src/RaycastManager.cpp
    src/Render2D.cpp
    src/Render3D.cpp
    src/RenderFlags.cpp
    src/RenderGraph.cpp
    src/RenderGraphBuilder.cpp
    src/RenderPass.cpp
    src/Renderer.cpp
    src/RendererStateSaver.cpp
    src/RigidBody.cpp
    src/SceneGraph.cpp
    src/Shader.cpp
    src/ShaderManager.cpp
    src/ShadowManager.cpp
    src/ShadowPass.cpp
    src/SkydomePass.cpp
    src/Sphere.cpp
    src/TeapotMesh.cpp
    src/TextureManager.cpp
    src/TextureMaterial.cpp
    src/TimeControlledRB.cpp
    src/TrianglePrimitive.cpp
    src/UIPass.cpp
    src/Vector3.cpp
    src/VectorTemplated.cpp
)

# target link libraries
target_link_libraries(hawthorne_engine
  PRIVATE
    game_libraries
)
