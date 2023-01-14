#include "RendererApp.h"
#include "Renderer.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#define USE_FRAMEBUFFER 1
#define STATISTICS 1


using namespace RendererSpace;

RendererSpace::RendererApp* RendererSpace::RendererApp::s_instance = nullptr;

RendererSpace::RendererApp::RendererApp() {
    ASSERT(!s_instance, "Renderer Application already exist");
    s_instance = this;

    // Init log
    Logger::init();

    // Create window and init glfw
    m_window = Window::createWindow(WindowProps("Renderer Application", 1600, 900));
    m_window->setEventCallback(BIND_EVENT_FUN(RendererApp::onEvent));

    // Create ImGui context
    m_window->createImGuiContext();

#if USE_FRAMEBUFFER
    // create Framebuffer
    FrameBufferSpecification spec;
    spec.Width = 1280;
    spec.Height = 720;
    spec.Samples = 1;
    spec.Attachments = {
            FrameBufferTextureFormat::RGBA8,
//            FrameBufferTextureFormat::RGB8,
//            FrameBufferTextureFormat::RED_INTEGER,
            FrameBufferTextureFormat::Depth,
            };
    m_frameBuffer = FrameBuffer::createFrameBuffer(spec);
#endif

    // Init Renderer
    Renderer::init();

    // Create Renderer Camera
    m_rendererCamera = createRef<RendererCamera>();
}

RendererSpace::RendererApp::~RendererApp() {
    m_window->destroyImGuiContext();
}

void RendererSpace::RendererApp::run() {
    while(!b_stop) {
        onUpdate();

        beginImGuiFrame();
        onImGuiRender();
        endImGuiFrame();

        m_window->onUpdate();
    }
}

void RendererSpace::RendererApp::onUpdate() {
#if USE_FRAMEBUFFER
    if(FrameBufferSpecification spec = m_frameBuffer->getSpecification(); m_viewportSize.x > 0.0f &&
                                                                          m_viewportSize.y > 0.0f &&
                                                                          (spec.Width != m_viewportSize.x ||
                                                                           spec.Height != m_viewportSize.y)) {
        m_frameBuffer->resize((uint32_t) m_viewportSize.x, (uint32_t) m_viewportSize.y);
        m_rendererCamera->setViewportSize(m_viewportSize.x, m_viewportSize.y);
    }
    m_frameBuffer->bind();
//    m_frameBuffer->clearAttachment(1, -1);
#endif

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(.3, .4, .5, .5);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    m_rendererCamera->onUpdate();

    Renderer::beginScene(m_rendererCamera);
    Renderer::drawModel();
    Renderer::endScene();

#if USE_FRAMEBUFFER
    m_frameBuffer->unbind();
#endif
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


void RendererSpace::RendererApp::onImGuiRender() {
#if USE_FRAMEBUFFER
    enableImGuiDocking();
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");

        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        uint64_t textureID = m_frameBuffer->getColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_viewportSize.x, m_viewportSize.y },
                     ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        ImGui::End();
        ImGui::PopStyleVar();
    }
#endif

#if STATISTICS
    statImGuiRender();
#endif
}

void RendererSpace::RendererApp::onEvent(Event& event) {
    using namespace RendererSpace;
    EventDispatcher eventDispatcher(event);
    // cause dispatch needs a functional object rather than a pure function pointer, we need to
    // use std::bind to convert a function pointer to a functional object
    eventDispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FUN(RendererApp::onWindowClose));
    eventDispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FUN(RendererApp::onWindowResize));
    m_rendererCamera->onEvent(event);
}

bool RendererSpace::RendererApp::onWindowClose(WindowCloseEvent &event) {
    b_stop = true;
    return true;
}

bool RendererSpace::RendererApp::onWindowResize(WindowResizeEvent &event) {
    LOG_TRACE("Window resize to ({}, {})", event.getWidth(), event.getHeight());
    return true;
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

    if(opt_fullscreen)
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

void RendererApp::statImGuiRender() {
    ImGui::Begin("Renderer Statistics");

    auto& stat = Renderer::getStat();
    ImGui::Text("Vertex count: %d", stat.VertexCount);
    ImGui::Text("Index count: %d", stat.IndexCount);
    ImGui::Text("Triangle count: %d", stat.TriangleCount);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
}

