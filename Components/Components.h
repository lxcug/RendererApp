// @author: lx
// @time: 2023/1/18
// @brief: 

#ifndef RENDERER_COMPONENTS_H
#define RENDERER_COMPONENTS_H

#include <utility>

#include "Core/Core.h"
#include "Core/GUID.h"
#include "Core/Object.h"
#include "glm.hpp"
#include "Buffers/Texture.h"
#include "glm/gtc/matrix_transform.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include"glm/gtx/quaternion.hpp"

namespace RendererSpace {
    struct TransformComponent {
        glm::vec3 Translation = {0.f, 0.f, 0.f};
        glm::vec3 Rotation = {0.f, 0.f, 0.f};
        glm::vec3 Scale = {1.f, 1.f, 1.f};

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& translation): Translation(translation) {}

        glm::mat4 getTransform() const {
            glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
            return glm::translate(glm::mat4(1.f), Translation) * rotation * glm::scale(glm::mat4(1.f), Scale);
        }
    };

    struct TagComponent {
        std::string Tag;

        TagComponent() = default;
        TagComponent(const TagComponent& tagComponent): Tag(tagComponent.Tag) {}
        TagComponent(std::string tag): Tag(std::move(tag)) {}
    };

    struct GUIDComponent {
        GUID ID;

        GUIDComponent() = default;
        GUIDComponent(GUID guid): ID(guid) {}
        GUIDComponent(const GUIDComponent&) = default;
    };

    struct SpriteRendererComponent {
        glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
        float TilingFactor = 1.f;
        Ref<Texture2D> Texture = nullptr;

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& color): Color(color) {}
        SpriteRendererComponent(const glm::vec4& color, Ref<Texture2D>& texture): Color(color), Texture(texture) {}
    };

    struct ObjectComponent {
        ObjectComponent(Object object): Obj(std::move(object)) {}
        ObjectComponent(Ref<Object>& object): Obj(*object) {}

        Object& getObject() {
            return Obj;
        }

        Object Obj;
    };
}




#endif //RENDERER_COMPONENTS_H
