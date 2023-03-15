#include "RendererCamera.h"
#include "OpenGLInput.h"


void RendererSpace::RendererCamera::updateProjection() {
    m_aspectRatio = m_viewportWidth / m_viewportHeight;
    m_projection = glm::perspective(glm::radians(m_FOV), m_aspectRatio, m_nearClip, m_farClip);
}

void RendererSpace::RendererCamera::updateView() {
    m_position = calculatePosition();
    glm::quat orientation = getOrientation();
    m_view = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(orientation);
    m_view = glm::inverse(m_view);

    LOG_INFO("focal point {} {} {}", m_focalPoint.x, m_focalPoint.y, m_focalPoint.z);
    LOG_INFO("camera pos {} {} {}", m_position.x, m_position.y, m_position.z);
}

glm::vec3 RendererSpace::RendererCamera::calculatePosition() const {
    return m_focalPoint - getForwardDirection() * m_distance;  // get camera position
}

bool RendererSpace::RendererCamera::onMouseScroll(RendererSpace::MouseScrolledEvent &event) {
    float delta = event.getYOffset() * 0.1f;
    mouseZoom(delta);
    updateView();
    return false;
}

void RendererSpace::RendererCamera::mousePan(const glm::vec2 &delta) {
    auto [xSpeed, ySpeed] = panSpeed();
    // move focalPoint
    m_focalPoint += -getRightDirection() * delta.x * xSpeed * m_distance;
    m_focalPoint += getUpDirection() * delta.y * ySpeed * m_distance;
}

void RendererSpace::RendererCamera::mouseRotate(const glm::vec2 &delta) {
    float yawSign = getUpDirection().y < 0 ? -1.0f : 1.0f;
    m_yaw += yawSign * delta.x * rotationSpeed();
    m_pitch += delta.y * rotationSpeed();
}

void RendererSpace::RendererCamera::mouseZoom(float delta) {
    // increase distance between focalPoint and targetPoint
    m_distance -= delta * zoomSpeed();
    if(m_distance < 1.0f) {  // if dis < 1.f, move focalPoint
        m_focalPoint += getForwardDirection() * zoomSpeed();
        m_distance = 1.0f;
    }
//    m_FOV -= delta * zoomSpeed();
//    updateProjection();
}

std::pair<float, float> RendererSpace::RendererCamera::panSpeed() const {
    float x = std::min(m_viewportWidth / 1000.0f, 2.4f);  // max = 2.4f
    float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;
    float y = std::min(m_viewportHeight / 1000.0f, 2.4f);  // max = 2.4f
    float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

    return { xFactor, yFactor };
}

float RendererSpace::RendererCamera::zoomSpeed() const {
    float distance = m_distance * 0.2f;
    distance = std::max(distance, 0.0f);
    float speed = distance * distance;
    speed = std::min(speed, 100.0f);  // max speed = 100
    return speed;
}

void RendererSpace::RendererCamera::onUpdate() {
    if(Input::isKeyPressed(Key::LeftAlt)) {
        const glm::vec2& mousePos = Input::getMousePosition();
        glm::vec2 delta = (mousePos - m_initialMousePosition) * 0.003f;
        m_initialMousePosition = mousePos;

        if(Input::isMouseButtonPressed(Mouse::ButtonMiddle)) {
            mousePan(delta);
        }
        if(Input::isMouseButtonPressed(Mouse::ButtonLeft)) {
            mouseRotate(delta);
        }
        if(Input::isMouseButtonPressed(Mouse::ButtonRight)) {
            mouseZoom(delta.y);
        }
    }
    updateView();
}

void RendererSpace::RendererCamera::onEvent(RendererSpace::Event &event) {
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FUN(RendererCamera::onMouseScroll));
}
