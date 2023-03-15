#include "RendererScene.h"

#include <utility>
#include "Entity.h"
#include "Core/Renderer.h"


namespace RendererSpace {
    RendererScene::RendererScene() {
        Entity model1 = createModel(GUID(), "../Assets/Models/spot/spot_triangulated_good.obj", "model 1");
        model1.getComponent<TransformComponent>().Translation = {1., 1., 0.};

        Entity model2 = createModel(GUID(), "../Assets/Models/cube/cube.obj", "model 2");
    }

    Entity RendererScene::createEntity(GUID guid, const std::string& name) {
        Entity entity = {m_registry.create(), this};
        entity.addComponent<TagComponent>(name);
        entity.addComponent<GUIDComponent>(guid);
        entity.addComponent<TransformComponent>();
        return entity;
    }

    Entity RendererScene::createModel(GUID guid, const std::string &filepath, const std::string &name) {
        Entity entity = {m_registry.create(), this};
        entity.addComponent<TagComponent>(name);
        entity.addComponent<GUIDComponent>(guid);
        entity.addComponent<TransformComponent>();
        Object obj;
        obj.loadObject(filepath, entity);
        entity.addComponent<ObjectComponent>(obj);
        return entity;
    }

    void RendererScene::destroyEntity(Entity& entity) {
        m_registry.destroy(entity);
    }

    void RendererScene::renderScene(Ref<RendererCamera> camera) {
        Renderer::beginScene(std::move(camera));
        auto view = m_registry.view<TransformComponent, ObjectComponent>();
        for(auto entity : view) {
            auto [transform, object] = view.get<TransformComponent, ObjectComponent>(entity);

            Renderer::drawModel(object.getObject(), transform.getTransform());
        }
        Renderer::endScene();
    }
}
