cmake_minimum_required(VERSION 3.24)
project(Renderer)

set(CMAKE_CXX_STANDARD 14)


include_directories(vendor)
include_directories(./)
include_directories(vendor/spdlog/include)
include_directories(vendor/Glad/include)


add_executable(Renderer main.cpp
        Core/Logger.cpp Core/Logger.h
        Core/Core.h
        Core/Timer.cpp Core/Timer.h
        Core/Window.cpp Core/Window.h
        Platform/OpenGL/OpenGLWindow.cpp
        Platform/OpenGL/OpenGLWindow.h
        Core/Renderer.cpp Core/Renderer.h
        Buffers/GraphicsContex.cpp Buffers/GraphicsContex.h
        Platform/OpenGL/OpenGLContext.cpp Platform/OpenGL/OpenGLContext.h
        vendor/Glad/src/glad.c vendor/Glad/include/glad/glad.h
        Core/RendererApp.cpp Core/RendererApp.h)


find_library(GLFW glfw3 ${PROJECT_SOURCE_DIR}/vendor/GLFW/lib-mingw-w64)
find_library(GLEW glew32s ${PROJECT_SOURCE_DIR}/vendor/glew)

MESSAGE(STATUS "glew: " ${GLEW})
MESSAGE(STATUS "glfw: " ${GLFW})
target_link_libraries(Renderer
        ${GLFW}
        ${GLEW}
        opengl32.dll
        )
