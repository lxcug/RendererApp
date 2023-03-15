// @author: lx
// @time: 2023/1/23
// @brief: 

#ifndef RENDERER_PROJECTPATHS_H
#define RENDERER_PROJECTPATHS_H

#include "Core.h"
#include "direct.h"
#include <algorithm>


namespace RendererSpace {
    class ProjectPaths {
    public:
        static void init() {
            int maxLen = 100;
            char buffer[maxLen];
            getcwd(buffer, maxLen);
            m_projectPath = buffer;
            std::replace(m_projectPath.begin(), m_projectPath.end(), '\\', '/');
            size_t slashPos = m_projectPath.find_last_of('/');
            m_assetPath = m_projectPath.substr(0, slashPos) + "/Asset";
            m_corePath = m_projectPath.substr(0, slashPos) + "/Core";
        }

        [[nodiscard]] static std::string getProjectPath() { return m_projectPath; }
        [[nodiscard]] static std::string getAssetPath() { return m_assetPath; }
        [[nodiscard]] static std::string getCorePath() { return m_corePath; }

    private:
        static std::string m_projectPath;
        static std::string m_assetPath;
        static std::string m_corePath;
    };
}

#endif //RENDERER_PROJECTPATHS_H
