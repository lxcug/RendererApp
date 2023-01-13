#include "GLFW/glfw3.h"
#include "Core/Logger.h"
#include "Core/Renderer.h"
#include "Core/RendererApp.h"
#include "Buffers/Shader.h"


using namespace RendererSpace;


int main() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    RendererApp app;
    app.run();

    return 0;
}
