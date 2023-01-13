#include "GLFW/glfw3.h"
#include "OpenGLInput.h"
#include "RendererApp.h"


namespace RendererSpace{

    bool Input::isKeyPressed(KeyCode keyCode) {
        auto window = (GLFWwindow*)RendererApp::getApplication()->getWindow()->getNativeWindow();
        auto state = glfwGetKey(window, static_cast<int>(keyCode));
        return state == GLFW_PRESS;
    }

    bool Input::isMouseButtonPressed(MouseCode mouseCode) {
        auto window = (GLFWwindow*)RendererApp::getApplication()->getWindow()->getNativeWindow();
        auto state = glfwGetMouseButton(window, static_cast<int>(mouseCode));
        return state == GLFW_PRESS;
    }

    glm::vec2 Input::getMousePosition() {
        auto window = (GLFWwindow*)RendererApp::getApplication()->getWindow()->getNativeWindow();
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return {(float)xPos, (float)yPos};
    }

    float Input::getMouseX() {
        return getMousePosition().x;
    }

    float Input::getMouseY() {
        return getMousePosition().y;
    }
}
