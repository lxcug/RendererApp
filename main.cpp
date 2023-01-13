#include "Core/Logger.h"
#include "Core/Renderer.h"
#include "Core/RendererApp.h"


using namespace RendererSpace;


int main() {
    Logger::init();
    Renderer::init();

    RendererApp app;
    app.run();

    return 0;
}
