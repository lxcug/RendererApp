#include "OpenGLContext.h"


namespace RendererSpace {

    OpenGLContext::OpenGLContext(GLFWwindow *windowHandle): m_windowHandle(windowHandle) {
        ASSERT(windowHandle, "Window handle is nullptr");
    }

    void OpenGLContext::init() {
        glfwMakeContextCurrent(m_windowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        ASSERT(status, "Failed to initialize Glad");
    }

    void OpenGLContext::swapBuffers() {
        glfwSwapBuffers(m_windowHandle);
    }
}
