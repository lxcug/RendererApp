// @author: lx
// @time: 2023/1/14
// @brief: 

#ifndef RENDERER_OPENGLINPUT_H
#define RENDERER_OPENGLINPUT_H

#include "Core.h"
#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"
#include "glm/glm.hpp"


namespace RendererSpace {
    class Input {
    public:
        static bool isKeyPressed(KeyCode keyCode);

        static bool isMouseButtonPressed(MouseCode mouseCode);

        static glm::vec2 getMousePosition();
        static float getMouseX();
        static float getMouseY();
    };
}


#endif //RENDERER_OPENGLINPUT_H
