cmake_minimum_required(VERSION 3.24)
project(Renderer)

set(CMAKE_CXX_STANDARD 17)


include_directories(vendor)
include_directories(vendor/glm)
include_directories(./)
include_directories(vendor/imgui)
include_directories(vendor/SPIRV-Cross)
include_directories(vendor/shaderc/libshaderc/include)
include_directories(vendor/spdlog/include)
include_directories(vendor/Glad/include)
include_directories(vendor/ImGuizmo)
include_directories(vendor/entt/include)


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
        Core/RendererApp.cpp Core/RendererApp.h
        Core/KeyCodes.h Core/MouseCodes.h
        Events/Event.cpp Events/KeyEvent.cpp Events/ApplicationEvent.cpp Events/MouseEvent.cpp
        Buffers/Buffer.cpp Platform/OpenGL/OpenGLBuffer.cpp
        Buffers/VertexArray.cpp Platform/OpenGL/OpenGLVertexArray.cpp
        Buffers/Shader.cpp Platform/OpenGL/OpenGLShader.cpp

        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui.h
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui.cpp
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imconfig.h
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_internal.h
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp
        ${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.h
        ${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_glfw.cpp
        ${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.h
        ${PROJECT_SOURCE_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp
        ${PROJECT_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp

        ${PROJECT_SOURCE_DIR}/vendor/stb_image/stb_image.cpp

        Core/OBJ_Loader.h
        Buffers/UniformBuffer.cpp Platform/OpenGL/OpenGLUniformBuffer.cpp
        Core/RendererCamera.cpp Core/RendererCamera.h
        Core/OpenGLInput.cpp Core/OpenGLInput.h
        Buffers/Texture.cpp Platform/OpenGL/OpenGLTexture.cpp
        Buffers/FrameBuffer.cpp Platform/OpenGL/OpenGLFrameBuffer.cpp
        Core/Mesh.cpp Core/Mesh.h Core/GlobalSettings.h Core/GlobalSettings.cpp
        ${PROJECT_SOURCE_DIR}/vendor/ImGuizmo/ImGuizmo.h
        ${PROJECT_SOURCE_DIR}/vendor/ImGuizmo/ImGuizmo.cpp
        Components/Components.cpp Components/Components.h
        Core/ProjectPaths.cpp Core/ProjectPaths.h
        Core/GUID.cpp Core/GUID.h Scene/RendererScene.cpp Scene/RendererScene.h Scene/Entity.cpp Scene/Entity.h Core/Object.cpp Core/Object.h)


find_library(GLFW glfw3 ${PROJECT_SOURCE_DIR}/vendor/GLFW/lib-mingw-w64)
find_library(GLEW glew32s ${PROJECT_SOURCE_DIR}/vendor/glew)
find_library(SHADERC shaderc_shared ${PROJECT_SOURCE_DIR}/vendor/shaderc)

MESSAGE(STATUS "glew: " ${GLEW})
MESSAGE(STATUS "glfw: " ${GLFW})
MESSAGE(STATUS "shaderc " ${SHADERC})

target_link_libraries(Renderer
        ${GLFW}
        ${GLEW}
        ${SHADERC}

        ${PROJECT_SOURCE_DIR}/vendor/VulKan/libspirv-cross-core.a
        ${PROJECT_SOURCE_DIR}/vendor/VulKan/libspirv-cross-glsl.a

        opengl32.dll
        )
