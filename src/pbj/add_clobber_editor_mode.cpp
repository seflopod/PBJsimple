
#ifdef PBJ_EDITOR

#include "pbj/add_clobber_editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

Id AddClobberEditorMode::id_("EditorMode.add_clobber");


AddClobberEditorMode::AddClobberEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.add_clob_btn"))))
{
    editor_.getUIElement(Id("menu.add_clob_panel"))->setVisible(true);
    editor_.getUIElement(Id("menu.material_panel"))->setVisible(true);
}

AddClobberEditorMode::~AddClobberEditorMode()
{
    editor_.getUIElement(Id("menu.add_clob_panel"))->setVisible(false);
    editor_.getUIElement(Id("menu.material_panel"))->setVisible(false);
}

const Id& AddClobberEditorMode::getId() const
{
    return id_;
}

void AddClobberEditorMode::onClick(I32 button, const vec2& start, const vec2& end)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        scene::Entity::EntityType type = (editor_.getActiveMaterial() == "spawnpoint") ? scene::Entity::SpawnPoint : scene::Entity::Terrain;

        scene::Entity* e = new scene::Entity();
        e->setType(type);
        e->setShape(new ShapeSquare());
        e->getTransform().setPosition(end);
        e->getTransform().setScale(type == scene::Entity::SpawnPoint ? vec2(1, 2) : editor_.getActiveScale());
        e->getTransform().setRotation(0);
        e->setMaterial(&getEngine().getResourceManager().getMaterial(
            sw::ResourceId(Id(PBJ_ID_PBJBASE), Id(editor_.getActiveMaterial()))));
        e->enableDraw();

        editor_.addEntity(std::unique_ptr<scene::Entity>(e));
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        auto result = editor_.getClosestEntity(end, true, true);

        if (result.first == nullptr)
            return;

        if (result.second / editor_.getZoom() < 8)
            editor_.removeEntity(result.first);
    }
}

} // namespace pbj

#endif
