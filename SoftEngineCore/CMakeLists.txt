cmake_minimum_required(VERSION 3.24.1 FATAL_ERROR)

set(ENGINE_PROJECT_NAME SoftEngineCore)

set(ENGINE_PUBLIC_INCLUDES
	includes/SoftEngineCore/Application.hpp
	includes/SoftEngineCore/Log.hpp
	includes/SoftEngineCore/Event.hpp
	includes/SoftEngineCore/Camera.hpp
	includes/SoftEngineCore/Keys.hpp
	includes/SoftEngineCore/Input.hpp
)

set(ENGINE_PRIVATE_INCLUDES
	src/SoftEngineCore/Window.hpp
	src/SoftEngineCore/Rendering/OpenGL/ShaderProgram.hpp
	src/SoftEngineCore/Rendering/OpenGL/VertexBuffer.hpp
	src/SoftEngineCore/Rendering/OpenGL/VertexArray.hpp
	src/SoftEngineCore/Rendering/OpenGL/IndexBuffer.hpp
	src/SoftEngineCore/Rendering/OpenGL/Renderer_OpenGl.hpp
	src/SoftEngineCore/Modules/UIModule.hpp
	src/SoftEngineCore/Rendering/OpenGL/Texture2D.hpp
)

set(ENGINE_PRIVATE_SOURCES
	src/SoftEngineCore/Application.cpp
	src/SoftEngineCore/Window.cpp
	src/SoftEngineCore/Rendering/OpenGL/ShaderProgram.cpp
	src/SoftEngineCore/Rendering/OpenGL/VertexBuffer.cpp
	src/SoftEngineCore/Rendering/OpenGL/VertexArray.cpp
	src/SoftEngineCore/Rendering/OpenGL/IndexBuffer.cpp
	src/SoftEngineCore/Camera.cpp
	src/SoftEngineCore/Rendering/OpenGL/Renderer_OpenGl.cpp
	src/SoftEngineCore/Modules/UIModule.cpp
	src/SoftEngineCore/Input.cpp
	src/SoftEngineCore/Rendering/OpenGL/Texture2D.cpp
)

set(ENGINE_ALL_SOURCES
	${ENGINE_PUBLIC_INCLUDES}
	${ENGINE_PRIVATE_INCLUDES}
	${ENGINE_PRIVATE_SOURCES}
)

add_library(${ENGINE_PROJECT_NAME} STATIC
	${ENGINE_ALL_SOURCES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
	${ENGINE_ALL_SOURCES}
)

target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
target_include_directories(${ENGINE_PROJECT_NAME} PRIVATE src)
target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)

add_subdirectory(../external/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE spdlog)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glad)

add_subdirectory(../external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glm)

add_subdirectory(../external/stb ${CMAKE_CURRENT_BINARY_DIR}/stb)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE stb)

set(IMGUI_INCLUDES
	../external/imgui/imgui.h
	../external/imgui/backends/imgui_impl_glfw.h
	../external/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
	../external/imgui/imgui.h
	../external/imgui/imgui.cpp
	../external/imgui/backends/imgui_impl_glfw.cpp
	../external/imgui/backends/imgui_impl_opengl3.cpp
	../external/imgui/imgui_draw.cpp
	../external/imgui/imgui_tables.cpp
	../external/imgui/imgui_widgets.cpp
	../external/imgui/imgui_demo.cpp
)

add_library(ImGui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)

target_include_directories(ImGui PUBLIC ../external)
target_include_directories(ImGui PUBLIC ../external/imgui)
target_link_libraries(ImGui PRIVATE glad glfw)

target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE ImGui)

