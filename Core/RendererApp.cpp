#include "RendererApp.h"


RendererSpace::RendererApp::RendererApp() {
    m_window = Window::createWindow(WindowProps("Renderer Application", 1600, 900));
}

RendererSpace::RendererApp::~RendererApp() {

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


