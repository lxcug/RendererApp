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
        onUpdate();
    }
}

void RendererSpace::RendererApp::onUpdate() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(.3, .4, .5, .5);
    onImGui();
    m_window->onUpdate();
}

void RendererSpace::RendererApp::beginImGuiFrame() const {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void RendererSpace::RendererApp::endImGuiFrame() const {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
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

void RendererSpace::RendererApp::onImGui() {
    beginImGuiFrame();
    enableImGuiDocking();

    {
        ImGui::Begin("Renderer Settings");
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::End();
    }

    endImGuiFrame();
}

void RendererSpace::RendererApp::enableImGuiDocking() {
    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;
    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;

    if(io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    style.WindowMinSize.x = minWinSizeX;
}

