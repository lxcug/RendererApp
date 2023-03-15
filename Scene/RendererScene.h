// @author: lx
// @time: 2023/2/25
// @brief: 

#ifndef RENDERER_RENDERERSCENE_H
#define RENDERER_RENDERERSCENE_H

#include "Core/Core.h"
#include "entt.hpp"
#include "Components/Components.h"
#include "Core/RendererCamera.h"


namespace RendererSpace {
    class Entity;
    class RendererScene {
    public:
        RendererScene();
        ~RendererScene() = default;

        friend class Entity;

        Entity createEntity(GUID guid, const std::string& name = "Unnamed entity");

        Entity createModel(GUID guid, const std::string& filepath, const std::string& name = "Unnamed entity");

        void destroyEntity(Entity& entity);

        void renderScene(Ref<RendererCamera> camera);

        template<typename... Components>
        auto getAllEntitiesWith() {
            return m_registry.view<Components...>();
        }

        entt::registry& getRegistry() {
            return m_registry;
        }

    private:
        entt::registry m_registry;
    };

}


#endif //RENDERER_RENDERERSCENE_H
