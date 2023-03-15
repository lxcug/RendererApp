// @author: lx
// @time: 2023/2/25
// @brief: 

#ifndef RENDERER_ENTITY_H
#define RENDERER_ENTITY_H

#include "Core/Core.h"
#include "Components/Components.h"
#include "Scene/RendererScene.h"
#include "Core/Logger.h"


namespace RendererSpace {
    class Entity {
    public:
        Entity() = default;
        Entity(entt::entity handle, RendererScene* scene): m_entityHandle(handle), m_scene(scene) {}

        Entity(const Entity& entity): m_entityHandle(entity.m_entityHandle), m_scene(entity.m_scene) {}

        template<typename T, typename... Args>
        T& addComponent(Args&&... args) {
            ASSERT(!hasComponent<T>(), "Entity already has the component!");
            T& component = m_scene->m_registry.emplace<T>(m_entityHandle, std::forward<Args>(args)...);
            return component;
        }

        template<typename T>
        T& getComponent() {
            return m_scene->m_registry.get<T>(m_entityHandle);
        }

        template<typename T>
        const T& getComponent() const {
            return m_scene->m_registry.get<T>(m_entityHandle);
        }

        template<typename T>
        bool hasComponent() {
            return m_scene->m_registry.has<T>(m_entityHandle);
        }

        template<typename T>
        bool hasComponent() const {
            return m_scene->m_registry.has<T>(m_entityHandle);
        }

        template<typename T, typename... Args>
        T& addOrReplaceComponent(Args&&... args) {
            T& component = m_scene->m_registry.emplace_or_replace<T>(m_entityHandle, std::forward<Args>(args)...);
            return component;
        }

        template<typename T>
        void removeComponent() {
            ASURA_CORE_ASSERT(hasComponent<T>(), "Entity doesn't have the component");
            m_scene->m_registry.remove<T>(m_entityHandle);
        }

        const std::string& getName() {
            return getComponent<TagComponent>().Tag;
        }

        operator bool() const {
            return m_entityHandle != entt::null;
        }
        operator int() const {
            return (int)m_entityHandle;
        }
        operator uint32_t() const {
            return (uint32_t)m_entityHandle;
        }
        bool operator==(const Entity& other) {
            return m_entityHandle == other.m_entityHandle && m_scene == other.m_scene;
        }
        bool operator!=(const Entity& other) {
            return !(*this == other);
        }
        operator entt::entity() const {
            return m_entityHandle;
        }
        GUID getGUID() const {
            return getComponent<GUIDComponent>().ID;
        }

    private:
        entt::entity m_entityHandle{ entt::null };
        RendererScene* m_scene = nullptr;
};
}


#endif //RENDERER_ENTITY_H
