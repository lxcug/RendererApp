#include "Core/Core.h"
#include "Core/Logger.h"
#include "Core/Timer.h"
#include "Platform/OpenGL/OpenGLWindow.h"
#include "Core/Renderer.h"


int main() {
    using namespace RendererSpace;
    Logger::init();
    Renderer::init();

    WindowProps props("Renderer", 1920, 1080);
    Ref<Window> window = Window::createWindow(props);
    while(true) {
        window->onUpdate();
    }

    return 0;
}
