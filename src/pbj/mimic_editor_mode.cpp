///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/mimic_editor_mode.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::MimicEditorMode functions.

#ifdef PBJ_EDITOR

#include "pbj/mimic_editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Identifies this editor mode and allows the safe casting of an
///         EditorMode pointer or ref to a child type.
Id MimicEditorMode::id_("EditorMode.mimic");

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a MimicEditorMode object.
///
/// \details The construction of an EditorMode object signals that the Editor
///         has entered that mode.
///
/// \param  editor The editor object.
MimicEditorMode::MimicEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.mimic_btn"))))
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destroys a MimicEditorMode object.
///
/// \details The destruction of an EditorMode object signals that the Editor
///         is leaving that mode, so any active edits must be finalized or
///         cancelled.
MimicEditorMode::~MimicEditorMode()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns this EditorMode's Id.
///
/// \brief  The ID differentiates the editor mode and allows the safe casting
///         of an EditorMode pointer or ref to a child type.
///
/// \return The id_ static member.
const Id& MimicEditorMode::getId() const
{
    return id_;
}

} // namespace pbj

#endif
