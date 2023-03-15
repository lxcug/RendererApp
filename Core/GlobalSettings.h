// @author: lx
// @time: 2023/1/16
// @brief: 

#ifndef RENDERER_GLOBALSETTINGS_H
#define RENDERER_GLOBALSETTINGS_H


namespace RendererSpace {
    struct SettingType {
        bool SettingOption;
        bool NeedRestart;
    };
}


namespace RendererSpace {
    class GlobalSettings {
    public:
        static void init() {
            loadSettingsFromYaml();
            onUpdate();
        }

        // TODO load from yaml file and commit changes to the file
        static void loadSettingsFromYaml() {}
        static void changeSettings() { }

        static void onUpdate() {
            // TODO for some settings need to restart app, use onUpdate to reinitialize some class to implement the same effect
            // TODO setting relevant to OpenGL need to restart
        }

        // OpenGL Settings
        static bool b_enableBlend;
        static bool b_useFrameBuffer;
        static bool b_useDepthBuffer;
        static bool b_wireframeMode;


        // ImGui Settings
        static bool b_enableImGuiWidgetSettings;
        static bool b_enableImGuiWidgetStatistic;

        static bool b_placePointLight;
    };
}


#endif //RENDERER_GLOBALSETTINGS_H
