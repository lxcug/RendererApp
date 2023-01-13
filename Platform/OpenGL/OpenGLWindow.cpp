#include "OpenGLWindow.h"


bool RendererSpace::OpenGLWindow::s_glfwInit = false;

RendererSpace::OpenGLWindow::OpenGLWindow(RendererSpace::WindowProps &props) {
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

    LOG_INFO("createWindow window \"{0}\": ({1}, {2})", m_windowData.Title, m_windowData.Width, m_windowData.Height);
}

void RendererSpace::OpenGLWindow::onUpdate() {
    glfwPollEvents();
    m_context->swapBuffers();
}
