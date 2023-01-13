// @author: lx
// @time: 2023/1/13
// @brief: 

#ifndef RENDERER_RENDERERCAMERA_H
#define RENDERER_RENDERERCAMERA_H

#include "Core.h"
#include "Logger.h"
#include "Events/Event.h"
#include "glm.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"
#include "Events/MouseEvent.h"


namespace RendererSpace {
    class RendererCamera {
    public:
        RendererCamera() {
            m_projection = glm::perspective(glm::radians(m_FOV), m_aspectRatio, m_nearClip, m_farClip);
        }
        RendererCamera(float fov, float aspect, float near, float far) {
            m_projection = glm::perspective(glm::radians(m_FOV), m_aspectRatio, m_nearClip, m_farClip);
        }

        void onUpdate();
        void onEvent(Event& event);

        float getDistance() const {
            return m_distance;
        }
        void setDistance(float dis) {
            m_distance = dis;
        }

        void setViewportSize(float width, float height) {
            m_viewportWidth = width;
            m_viewportHeight = height;
            updateProjection();
        }

        const glm::mat4& getViewMatrix() const {
            return m_viewMatrix;
        }
        glm::mat4 getViewProjection() const {
            return m_projection * m_viewMatrix;
        };

        glm::vec3 getUpDirection() const { return glm::rotate(getOrientation(), glm::vec3(0.f, 1.f, 0.f)); }
        glm::vec3 getRightDirection() const { return glm::rotate(getOrientation(), glm::vec3(1.f, 0.f, 0.f)); }
        glm::vec3 getForwardDirection() const { return glm::rotate(getOrientation(), glm::vec3(0.f, 0.f, -1.f)); }
        const glm::vec3& getPosition() const {
            return m_position;
        }
        glm::quat getOrientation() const { return glm::quat(glm::vec3(-m_pitch, -m_yaw, 0.f)); }

        float getPitch() const { return m_pitch; }
        float getYaw() const { return m_yaw; }

    private:
        void updateProjection();
        void updateView();
        glm::vec3 calculatePosition() const;

        bool onMouseScroll(MouseScrolledEvent& event);
        void mousePan(const glm::vec2& delta);
        void mouseRotate(const glm::vec2& delta);
        void mouseZoom(float delta);
        std::pair<float, float> panSpeed() const;
        float rotationSpeed() const { return .8f; }
        float zoomSpeed() const;

    private:
        float m_FOV = 45.f, m_aspectRatio = 1.778f;
        float m_nearClip = .1f, m_farClip = 1000.f;

        glm::mat4 m_viewMatrix;
        glm::vec3 m_position = {0.f, 0.f, 0.f};
        glm::vec3 m_focalPoint = {0.f, 0.f, 0.f};
        glm::vec2 m_initialMousePosition = {0.f, 0.f};

        float m_distance = 10.f;
        float m_pitch = 0.f, m_yaw = 0.f;
        float m_viewportWidth = 1280.f, m_viewportHeight = 720.f;
        glm::mat4 m_projection = glm::mat4(1.f);
    };
}


#endif //RENDERER_RENDERERCAMERA_H
