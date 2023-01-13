#include "OpenGLWindow.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"


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
    m_windowData.bVSync = enable;
}


