///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/add_clobber_editor_mode.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::AddClobberEditorMode functions.

#ifdef PBJ_EDITOR

#include "pbj/add_clobber_editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Identifies this editor mode and allows the safe casting of an
///         EditorMode pointer or ref to a child type.
Id AddClobberEditorMode::id_("EditorMode.add_clobber");

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs an AddClobberEditorMode object.
///
/// \details The construction of an EditorMode object signals that the Editor
///         has entered that mode.
///
/// \param  editor The editor object.
AddClobberEditorMode::AddClobberEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.add_clob_btn"))))
{
    editor_.getUIElement(Id("menu.add_clob_panel"))->setVisible(true);
    editor_.getUIElement(Id("menu.material_panel"))->setVisible(true);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destroys an AddClobberEditorMode object.
///
/// \details The destruction of an EditorMode object signals that the Editor
///         is leaving that mode, so any active edits must be finalized or
///         cancelled.
AddClobberEditorMode::~AddClobberEditorMode()
{
    editor_.getUIElement(Id("menu.add_clob_panel"))->setVisible(false);
    editor_.getUIElement(Id("menu.material_panel"))->setVisible(false);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns this EditorMode's Id.
///
/// \brief  The ID differentiates the editor mode and allows the safe casting
///         of an EditorMode pointer or ref to a child type.
///
/// \return The id_ static member.
const Id& AddClobberEditorMode::getId() const
{
    return id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse button is clicked.
///
/// \details Right clicking removes the nearest entity to the cursor (within a
///         reasonable distance).  Left clicking adds a new entity at the
///         cursor's position.  The type of entity to add is determined by the
///         current selection made in the editor's material UI panel.
///
/// \param  button The button that was clicked.
/// \param  start  The mouse's position when the click began
/// \param  end    The mouse's position when the click ended  (should be the
///                same as start.
void AddClobberEditorMode::onClick(I32 button, const vec2& start, const vec2& end)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        scene::Entity::EntityType type = (editor_.getActiveMaterial() == "spawnpoint") ? scene::Entity::SpawnPoint : scene::Entity::Terrain;

        scene::Entity* e = new scene::Entity();
        e->setType(type);
        e->setShape(new gfx::ShapeSquare());
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
