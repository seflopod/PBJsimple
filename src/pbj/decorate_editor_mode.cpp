#ifdef PBJ_EDITOR

#include "pbj/decorate_editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

Id DecorateEditorMode::id_("EditorMode.decorate");


DecorateEditorMode::DecorateEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.decorate_btn"))))
{
    editor_.getUIElement(Id("menu.decorate_panel"))->setVisible(true);
    editor_.getUIElement(Id("menu.material_panel"))->setVisible(true);
}

DecorateEditorMode::~DecorateEditorMode()
{
    editor_.getUIElement(Id("menu.decorate_panel"))->setVisible(false);
    editor_.getUIElement(Id("menu.material_panel"))->setVisible(false);
}

const Id& DecorateEditorMode::getId() const
{
    return id_;
}

void DecorateEditorMode::onClick(I32 button, const vec2& start, const vec2& end)
{
    auto result = editor_.getClosestEntity(end, true, true);

    if (result.first == nullptr)
        return;

    if (result.second / editor_.getZoom() < 8)
    {
        if (editor_.getActiveMaterial() == "spawnpoint")
        {
            result.first->setType(scene::Entity::SpawnPoint);
            result.first->setMaterial(&getEngine().getResourceManager().getMaterial(
                sw::ResourceId(Id(PBJ_ID_PBJBASE), Id(editor_.getActiveMaterial()))));
            result.first->getTransform().setScale(1, 2);
            result.first->getTransform().setRotation(0);
        }
        else
        {
            result.first->setType(scene::Entity::Terrain);
            result.first->setMaterial(&getEngine().getResourceManager().getMaterial(
                sw::ResourceId(Id(PBJ_ID_PBJBASE), Id(editor_.getActiveMaterial()))));
        }
    }
}

} // namespace pbj

#endif
