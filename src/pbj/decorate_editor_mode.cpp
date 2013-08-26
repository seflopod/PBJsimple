///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/decorate_editor_mode.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::DecorateEditorMode functions.

#ifdef PBJ_EDITOR

#include "pbj/decorate_editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Identifies this editor mode and allows the safe casting of an
///         EditorMode pointer or ref to a child type.
Id DecorateEditorMode::id_("EditorMode.decorate");

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a DecorateEditorMode object.
///
/// \details The construction of an EditorMode object signals that the Editor
///         has entered that mode.
///
/// \param  editor The editor object.
DecorateEditorMode::DecorateEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.decorate_btn"))))
{
    editor_.getUIElement(Id("menu.decorate_panel"))->setVisible(true);
    editor_.getUIElement(Id("menu.material_panel"))->setVisible(true);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destroys a DecorateEditorMode object.
///
/// \details The destruction of an EditorMode object signals that the Editor
///         is leaving that mode, so any active edits must be finalized or
///         cancelled.
DecorateEditorMode::~DecorateEditorMode()
{
    editor_.getUIElement(Id("menu.decorate_panel"))->setVisible(false);
    editor_.getUIElement(Id("menu.material_panel"))->setVisible(false);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns this EditorMode's Id.
///
/// \brief  The ID differentiates the editor mode and allows the safe casting
///         of an EditorMode pointer or ref to a child type.
///
/// \return The id_ static member.
const Id& DecorateEditorMode::getId() const
{
    return id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse button is clicked.
///
/// \details Works exactly like the left click (add) functionality of
///         AddClobberEditorMode except instead of adding a new entity, the
///         closest existing entity is changed.
///
/// \param  button The button that was clicked.
/// \param  start  The mouse's position when the click began
/// \param  end    The mouse's position when the click ended  (should be the
///                same as start.
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
