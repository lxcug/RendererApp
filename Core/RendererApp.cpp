#include "RendererApp.h"
#include "Renderer.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "glm/gtc/type_ptr.hpp"
#include "ImGuizmo.h"
#include "OpenGLInput.h"
#include "Core/ProjectPaths.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/matrix_decompose.hpp"

#define READ_PIXEL_DATA 1


using namespace RendererSpace;

RendererSpace::RendererApp* RendererSpace::RendererApp::s_instance = nullptr;

RendererSpace::RendererApp::RendererApp() {
    ASSERT(!s_instance, "Renderer Application already exist");
    s_instance = this;

    // Init log
    Logger::init();

    // Init path
    ProjectPaths::init();
//    LOG_INFO("{}", ProjectPaths::getProjectPath());
//    LOG_INFO("{}", ProjectPaths::getAssetPath());
//    LOG_INFO("{}", ProjectPaths::getCorePath());

    // Create window and init glfw
    m_window = Window::createWindow(WindowProps("Renderer Application", 1600, 900));
    m_window->setEventCallback(BIND_EVENT_FUN(RendererApp::onEvent));

    // Create ImGui context
    m_window->createImGuiContext();

    RendererSpace::GlobalSettings::init();

    if(GlobalSettings::b_useFrameBuffer) {
        // create Framebuffer
        FrameBufferSpecification spec;
        spec.Width = 1280;
        spec.Height = 720;
        spec.Samples = 1;
        spec.Attachments = {
                FrameBufferTextureFormat::RGBA8,
                FrameBufferTextureFormat::RED_INTEGER,
                FrameBufferTextureFormat::Depth,
        };
        m_frameBuffer = FrameBuffer::createFrameBuffer(spec);
    }

    // Init Renderer
    Renderer::init();
//    Renderer::loadOBJModel("../Assets/Models/spot/spot_triangulated_good.obj");

    m_scene = createRef<RendererScene>();

    // Create Renderer Camera
    m_rendererCamera = createRef<RendererCamera>();
}

RendererSpace::RendererApp::~RendererApp() {
    m_window->destroyImGuiContext();
}

void RendererSpace::RendererApp::run() {
    while(!b_stop) {
        onUpdate();
        RendererSpace::GlobalSettings::onUpdate();

        m_window->onUpdate();

        beginImGuiFrame();
        onImGuiRender();
        endImGuiFrame();
    }
}

void RendererSpace::RendererApp::onUpdate() {
    if(GlobalSettings::b_useFrameBuffer) {
        FrameBufferSpecification spec = m_frameBuffer->getSpecification();
        if(m_viewportSize.x > 0.0f && m_viewportSize.y > 0.0f && (spec.Width != m_viewportSize.x || spec.Height != m_viewportSize.y)) {
            m_frameBuffer->resize((uint32_t) m_viewportSize.x, (uint32_t) m_viewportSize.y);
            m_rendererCamera->setViewportSize(m_viewportSize.x, m_viewportSize.y);
        }
        m_frameBuffer->bind();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(m_settingsData.BackgroundColor.r, m_settingsData.BackgroundColor.g, m_settingsData.BackgroundColor.b,
                 m_settingsData.BackgroundColor.a);

    if(GlobalSettings::b_useFrameBuffer) {
        m_frameBuffer->clearAttachment(1, -1);
    }

    m_rendererCamera->onUpdate();
//    Renderer::beginScene(m_rendererCamera);
//    Renderer::drawModel();
//    Renderer::endScene();

    m_scene->renderScene(m_rendererCamera);

    auto[mx, my] = ImGui::GetMousePos();
    mx -= m_viewportBounds[0].x;
    my -= m_viewportBounds[0].y;

    glm::vec2 viewportSize = m_viewportSize;
    my = viewportSize.y - my;
    int mouseX = (int)mx;
    int mouseY = (int)my;

#if READ_PIXEL_DATA
    if(mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
        int pixelData = m_frameBuffer->readPixel(1, mouseX, mouseY);
        m_hoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, m_scene.get());
    }
#endif

    if(GlobalSettings::b_useFrameBuffer) {
        m_frameBuffer->unbind();
    }
}

void RendererSpace::RendererApp::beginImGuiFrame() const {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
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
    if(GlobalSettings::b_useFrameBuffer) {
        enableImGuiDocking();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
        ImGui::Begin("Viewport");
        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();
        m_viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
        m_viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
        m_viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
        uint64_t textureID = m_frameBuffer->getColorAttachmentRendererID();
        ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ m_viewportSize.x, m_viewportSize.y },
                     ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        ImGuizmoRender();

        ImGui::End();
        ImGui::PopStyleVar();
    }

    if(GlobalSettings::b_enableImGuiWidgetStatistic) {
        statImGuiRender();
    }

    if(GlobalSettings::b_enableImGuiWidgetSettings) {
        settingsImGuiRender();
    }
}

void RendererSpace::RendererApp::onEvent(Event& event) {
    using namespace RendererSpace;
    EventDispatcher eventDispatcher(event);
    // cause dispatch needs a functional object rather than a pure function pointer, we need to
    // use std::bind to convert a function pointer to a functional object
    m_rendererCamera->onEvent(event);
    eventDispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FUN(RendererApp::onWindowClose));
    eventDispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FUN(RendererApp::onWindowResize));
    eventDispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FUN(RendererApp::onKeyPressed));
    eventDispatcher.dispatch<MouseButtonPressedEvent>(BIND_EVENT_FUN(RendererApp::onMouseButtonPressed));
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

void RendererApp::statImGuiRender() {
    ImGui::Begin("Renderer Statistics");
    std::string name = "None";
    if(m_hoveredEntity) {
        name = m_hoveredEntity.hasComponent<TagComponent>() ? m_hoveredEntity.getComponent<TagComponent>().Tag : "None";
    }
    ImGui::Text("Hovered Entity: %s", name.c_str());

    auto& stat = Renderer::getStat();
    ImGui::Text("Vertex count: %d", stat.VertexCount);
    ImGui::Text("Index count: %d", stat.IndexCount);
    ImGui::Text("Triangle count: %d", stat.TriangleCount);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::End();
}

void RendererApp::settingsImGuiRender() {
    ImGui::Begin("Settings");

    ImGui::ColorEdit4("Background color", glm::value_ptr(m_settingsData.BackgroundColor));
    ImGui::Checkbox("Use frame buffer", &GlobalSettings::b_useFrameBuffer);
    ImGui::Checkbox("Use depth buffer", &GlobalSettings::b_useDepthBuffer);
    ImGui::Checkbox("Enable blend", &GlobalSettings::b_enableBlend);
    ImGui::Checkbox("Enable ImGui Statistic Widget", &GlobalSettings::b_enableImGuiWidgetStatistic);
    ImGui::Checkbox("Wireframe Mode", &GlobalSettings::b_wireframeMode);

    ImGui::Checkbox("Place Point Light", &GlobalSettings::b_placePointLight);
//    ImGui::Checkbox("Enable ImGui Settings Widget", &GlobalSettings::b_enableImGuiWidgetSettings);


    if(GlobalSettings::b_placePointLight) {
        ImGui::NewLine();
        ImGui::Text("Point Light Settings");


        auto pointLight = Renderer::getPointLight();
        ImGui::Text("Position %.2f %.2f %.2f", pointLight->getTransformedPosition().x,
                    pointLight->getTransformedPosition().y, pointLight->getTransformedPosition().z);
        ImGui::SliderFloat("Intensity", &pointLight->Intensity.x, 0, 50.);
        ImGui::SliderFloat("Ka", &pointLight->Ka.x, 0., .05);
        ImGui::SliderFloat("Ks", &pointLight->Ks.x, 0., .5);
        ImGui::SliderFloat("Kd", &pointLight->Kd.x, 0., .3);
    }
    ImGui::End();
}

void RendererApp::ImGuizmoRender() {
    if(m_selectedEntity && m_gizmoType != -1) {
        ImGuizmo::SetOrthographic(false);
        ImGuizmo::SetDrawlist();

        ImGuizmo::SetRect(m_viewportBounds[0].x, m_viewportBounds[0].y,
                          m_viewportBounds[1].x - m_viewportBounds[0].x,
                          m_viewportBounds[1].y - m_viewportBounds[0].y);

        const glm::mat4 &cameraProjection = m_rendererCamera->getProjection();
        glm::mat4 cameraView = m_rendererCamera->getView();

        // Entity transform
        auto& tc = m_selectedEntity.getComponent<TransformComponent>();
        glm::mat4 transform = tc.getTransform();

        // Snapping
        bool snap = Input::isKeyPressed(Key::LeftControl);
        float snapValue = 0.5f; // Snap to 0.5m for translation/scale
        // Snap to 45 degrees for rotation
        if(m_gizmoType == ImGuizmo::OPERATION::ROTATE)
            snapValue = 45.0f;

        float snapValues[3] = { snapValue, snapValue, snapValue };

        ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
                             (ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
                             nullptr, snap ? snapValues : nullptr);

        if(ImGuizmo::IsUsing()) {
            glm::vec3 translation, rotation, scale;
            decomposeTransform(transform, translation, rotation, scale);

            glm::vec3 deltaRotation = rotation - tc.Rotation;
            tc.Translation = translation;
            tc.Rotation += deltaRotation;
            tc.Scale = scale;
        }
    }

//    if(m_rendererCamera && m_gizmoType != -1) {
//        ImGuizmo::SetOrthographic(false);
//        ImGuizmo::SetDrawlist();
//
//        ImGuizmo::SetRect(m_viewportBounds[0].x, m_viewportBounds[0].y,
//                          m_viewportBounds[1].x - m_viewportBounds[0].x,
//                          m_viewportBounds[1].y - m_viewportBounds[0].y);
//
//        const glm::mat4 &cameraProjection = m_rendererCamera->getProjection();
//        glm::mat4 cameraView = m_rendererCamera->getView();
//
//
//        // Snapping
//        bool snap = Input::isKeyPressed(Key::LeftControl);
//        float snapValue = 0.5f; // Snap to 0.5m for translation/scale
//        // Snap to 45 degrees for rotation
//        if (m_gizmoType == ImGuizmo::OPERATION::ROTATE)
//            snapValue = 45.0f;
//
//        float snapValues[3] = {snapValue, snapValue, snapValue};
//
//        if(GlobalSettings::b_placePointLight) {
//            TransformComponent& pointLightTransComponent = Renderer::s_pointLight->TransformComponent;
//            glm::mat4 transform = pointLightTransComponent.getTransform();
//
//            ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
//                                 (ImGuizmo::OPERATION)m_gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
//                                 nullptr, snap ? snapValues : nullptr);
//
//            if(ImGuizmo::IsUsing()) {
//                glm::vec3 translation, rotation, scale;
//                decomposeTransform(transform, translation, rotation, scale);
//
//                glm::vec3 deltaRotation = rotation - Renderer::s_pointLight->TransformComponent.Rotation;
//                pointLightTransComponent.Translation = translation;
//                pointLightTransComponent.Rotation += deltaRotation;
//                pointLightTransComponent.Scale = scale;
//            }
//        }
//    }
}

bool RendererApp::decomposeTransform(const glm::mat4 &transform, glm::vec3 &outTranslation, glm::vec3 &outRotation,
                                     glm::vec3 &outScale) {
    using namespace glm;
    using T = float;

    mat4 LocalMatrix(transform);

    // Normalize the matrix.
    if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
        return false;

    // First, isolate perspective.  This is the messiest.
    if (
            epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
            epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
            epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>()))
    {
        // Clear the perspective partition
        LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
        LocalMatrix[3][3] = static_cast<T>(1);
    }

    // Next take care of translation (easy).
    outTranslation = vec3(LocalMatrix[3]);
    LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

    vec3 Row[3], Pdum3;

    // Now get scale and shear.
    for (length_t i = 0; i < 3; ++i)
        for (length_t j = 0; j < 3; ++j)
            Row[i][j] = LocalMatrix[i][j];

    // Compute X scale factor and normalize first row.
    outScale.x = length(Row[0]);
    Row[0] = detail::scale(Row[0], static_cast<T>(1));
    outScale.y = length(Row[1]);
    Row[1] = detail::scale(Row[1], static_cast<T>(1));
    outScale.z = length(Row[2]);
    Row[2] = detail::scale(Row[2], static_cast<T>(1));

    outRotation.y = asin(-Row[0][2]);
    if (cos(outRotation.y) != 0) {
        outRotation.x = atan2(Row[1][2], Row[2][2]);
        outRotation.z = atan2(Row[0][1], Row[0][0]);
    }
    else {
        outRotation.x = atan2(-Row[2][0], Row[1][1]);
        outRotation.z = 0;
    }
    return true;
}

bool RendererApp::onKeyPressed(KeyPressedEvent& event) {
    if(event.isRepeat())
        return false;

    switch (event.getKeyCode()) {
        // Gizmos
        case Key::Q: {
            if(!ImGuizmo::IsUsing())
                m_gizmoType = -1;
            break;
        }

        case Key::W: {
            if(!ImGuizmo::IsUsing())
                m_gizmoType = ImGuizmo::OPERATION::TRANSLATE;
            break;
        }

        case Key::E: {
            if(!ImGuizmo::IsUsing())
                m_gizmoType = ImGuizmo::OPERATION::ROTATE;
            break;
        }
        case Key::R: {
            if (!ImGuizmo::IsUsing())
                m_gizmoType = ImGuizmo::OPERATION::SCALE;
            break;
        }
    }
    return false;
}

bool RendererApp::onMouseButtonPressed(MouseButtonPressedEvent &event) {
    if(event.getMouseButton() == Mouse::ButtonLeft) {
        if(m_hoveredEntity && !ImGuizmo::IsOver())
            m_selectedEntity = m_hoveredEntity;

        if(!m_hoveredEntity && !ImGuizmo::IsOver()) {
            m_selectedEntity = Entity();
        }
    }
    return false;
}
