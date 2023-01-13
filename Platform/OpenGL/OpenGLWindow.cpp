#include "OpenGLWindow.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"

#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"


bool RendererSpace::OpenGLWindow::s_glfwInit = false;

RendererSpace::OpenGLWindow::OpenGLWindow(const RendererSpace::WindowProps &props) {
    m_windowData.Width = props.Width;
    m_windowData.Height = props.Height;
    m_windowData.Title = props.Title;

    // initialize glfw
    if(!s_glfwInit) {
        s_glfwInit = true;
        bool flag = glfwInit();
        ASSERT(flag, "Unable to initialize GLFW");
    }

    m_window = glfwCreateWindow((int)m_windowData.Width, (int)m_windowData.Height,
                                m_windowData.Title.c_str(), nullptr, nullptr);
    if(!m_window) {
        glfwTerminate();
    }

    // make context
    m_context = GraphicsContext::createGraphicsContext(m_window);
    m_context->init();
    // set user defined data structure
    glfwSetWindowUserPointer(m_window, &m_windowData);
    setVSync(true);

    setGLFWWindowEventCallback();
    setGLFWMouseEventCallback();
    setGLFWKeyEventCallback();
    setGLFWApplicationEventCallback();

    LOG_INFO("createWindow window \"{0}\": ({1}, {2})", m_windowData.Title, m_windowData.Width, m_windowData.Height);
}

void RendererSpace::OpenGLWindow::onUpdate() {
    glfwPollEvents();
    m_context->swapBuffers();
}

void RendererSpace::OpenGLWindow::setGLFWWindowEventCallback() {
    // set glfw window resize callback
    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height){
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowResizeEvent event(width, height);
        data.Width = width;
        data.Height = height;
        data.eventCallback(event);  // call WindowsWindow callback function
    });

    // set glfw window close callback
    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.eventCallback(event);  // call WindowsWindow callback function
    });

    // set glfw window move callback
    glfwSetWindowPosCallback(m_window, [](GLFWwindow* window, int xPos, int yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowMovedEvent event(xPos, yPos);
        data.eventCallback(event);
    });


    glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int focused) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (focused) {
            case GLFW_TRUE: {
                WindowFocusEvent event;
                data.eventCallback(event);
                break;
            }
            case GLFW_FALSE: {
                WindowLostFocusEvent event;
                data.eventCallback(event);
                break;
            }
        }
    });
}

void RendererSpace::OpenGLWindow::setGLFWMouseEventCallback() {
    // set glfw mouse button callback
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(button);
                data.eventCallback(event);
                break;
            }
            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(button);
                data.eventCallback(event);
                break;
            }
        }
    });

    // set glfw scroll event callback
    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseScrolledEvent event((float)xOffset, (float)yOffset);
        data.eventCallback(event);
    });

    // set glfw mouse move event callback
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseMovedEvent event((float)xPos, (float)yPos);
        data.eventCallback(event);
    });
}

void RendererSpace::OpenGLWindow::setGLFWKeyEventCallback() {
    // set glfw key events callback
    glfwSetKeyCallback(m_window,[](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        // press release and repeat
        switch(action) {
            case GLFW_PRESS: {
                KeyPressedEvent event(key, false);
                data.eventCallback(event);
                break;
            }
                break;
            case GLFW_RELEASE: {
                KeyReleasedEvent event(key);  // call WindowsWindow callback
                data.eventCallback(event);  // call WindowsWindow callback function
                break;
            }
            case GLFW_REPEAT: {
                KeyPressedEvent event(key, true);
                data.eventCallback(event);
                break;
            }
        }
    });
}

void RendererSpace::OpenGLWindow::setGLFWApplicationEventCallback() {

}

void RendererSpace::OpenGLWindow::setVSync(bool enable) {
    if(enable)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);
    m_windowData.VSync = enable;
}

void RendererSpace::OpenGLWindow::createImGuiContext() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    setDarkTheme();

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    const char* glsl_version = "#version 450";
    ImGui_ImplOpenGL3_Init(glsl_version);

    float fontSize = 18.0f;  // *2.0f;
    io.Fonts->AddFontFromFileTTF(
            "../Assets/Fonts/opensans/OpenSans-Bold.ttf", fontSize);
    io.FontDefault = io.Fonts->AddFontFromFileTTF(
            "../Assets/Fonts/consolas/Consolas-Regular.ttf", fontSize);
}

void RendererSpace::OpenGLWindow::destroyImGuiContext() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void RendererSpace::OpenGLWindow::setDarkTheme() {
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}


