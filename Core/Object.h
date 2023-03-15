// @author: lx
// @time: 2023/2/25
// @brief: 

#ifndef RENDERER_OBJECT_H
#define RENDERER_OBJECT_H

#include "Core.h"
#include "Mesh.h"


namespace RendererSpace {
    class Object {
    public:
        Object() = default;
        Object(const std::string &filepath);

        void loadObject(const std::string &filepath, int entityID=-1);

        std::vector<Mesh<Vertex>>& getMeshes() {
            return m_meshes;
        }

        void draw();

    private:
        std::vector<Mesh<Vertex>> m_meshes;
    };
}


#endif //RENDERER_OBJECT_H
