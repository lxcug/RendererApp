#include "RendererApp.h"
#include "Renderer.h"

#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"


RendererSpace::RendererApp::RendererApp() {
    Logger::init();
    Renderer::init();
    m_window = Window::createWindow(WindowProps("Renderer Application", 1600, 900));
    m_window->setEventCallback(BIND_EVENT_FUN(RendererApp::onEvent));
    m_window->createImGuiContext();
}

RendererSpace::RendererApp::~RendererApp() {
    m_window->destroyImGuiContext();
}

void RendererSpace::RendererApp::run() {
    while(!bStop) {
        beginImGui();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(.3, .4, .5, .5);

        {
            ImGui::Begin("Renderer Settings");
            ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
            ImGui::End();
        }

        endImGui();

        m_window->onUpdate();
    }
}

void RendererSpace::RendererApp::beginImGui() const {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void RendererSpace::RendererApp::endImGui() const {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
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


