#include "GLFW/glfw3.h"
#include "Core/RendererApp.h"

using namespace RendererSpace;


int main() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    RendererApp app;
    app.run();

    return 0;
}
