#include "RendererApp.h"
#include "Renderer.h"


RendererSpace::RendererApp::RendererApp() {
    Logger::init();
    Renderer::init();
    m_window = Window::createWindow(WindowProps("Renderer Application", 1600, 900));
    m_window->setEventCallback(BIND_EVENT_FUN(RendererApp::onEvent));
    createImGuiContext();
}

RendererSpace::RendererApp::~RendererApp() {
    destroyImGuiContext();
}

void RendererSpace::RendererApp::run() {
    while(!bStop) {
        beginImGui();
        endImGui();
        m_window->onUpdate();
    }
}

void RendererSpace::RendererApp::beginImGui() const {

}

void RendererSpace::RendererApp::endImGui() const {

}

void RendererSpace::RendererApp::createImGuiContext() {

}

void RendererSpace::RendererApp::destroyImGuiContext() {

}

void RendererSpace::RendererApp::onEvent(Event& event) {
    using namespace RendererSpace;
    EventDispatcher eventDispatcher(event);
    // cause dispatch needs a functional object rather than a pure function pointer, we need to
    // use std::bind to convert a function pointer to a functional object
    eventDispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FUN(RendererApp::onWindowClose));
    eventDispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FUN(RendererApp::onWindowResize));
}

bool RendererSpace::RendererApp::onWindowClose(WindowCloseEvent &event) {
    bStop = true;
    return true;
}

bool RendererSpace::RendererApp::onWindowResize(WindowResizeEvent &event) {
    LOG_TRACE("Window resize to ({}, {})", event.getWidth(), event.getHeight());
    return true;
}


