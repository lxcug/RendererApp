#include "Object.h"
#include "OBJ_Loader.h"

RendererSpace::Object::Object(const std::string &filepath) {
    loadObject(filepath);
}

void RendererSpace::Object::loadObject(const std::string &filepath, int entityID) {
    objl::Loader Loader;
    bool loadOut = Loader.LoadFile(filepath);
    if (!loadOut) {
        ASSERT(false, "Can't load obj file!");
        return;
    }
    LOG_INFO("Load {}", filepath);

    for (auto &mesh: Loader.LoadedMeshes) {
        LOG_INFO("Mesh name: {}", mesh.MeshName);
        std::vector<Vertex> vertices;
        std::vector<unsigned> indices;
        for(auto & vertex : mesh.Vertices) {
            vertices.emplace_back(
                    glm::vec3(vertex.Position.X, vertex.Position.Y, vertex.Position.Z),
                    glm::vec3(vertex.Normal.X, vertex.Normal.Y, vertex.Normal.Z),
                    glm::vec2(vertex.TextureCoordinate.X, vertex.TextureCoordinate.Y),
                    GlobalSettings::b_wireframeMode ? -1. : 0.,
                    entityID
            );
        }
        for(int j = 0; j < mesh.Indices.size(); j += 3) {
            indices.push_back(mesh.Indices[j]);
            indices.push_back(mesh.Indices[j + 1]);
            indices.push_back(mesh.Indices[j + 2]);
        }
        m_meshes.emplace_back(vertices, indices);
    }
}

void RendererSpace::Object::draw() {
    for(auto& mesh: m_meshes) {
        mesh.draw();
    }
}
