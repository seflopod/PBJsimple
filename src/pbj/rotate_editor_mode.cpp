#ifdef PBJ_EDITOR

#include "pbj/rotate_editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

Id RotateEditorMode::id_("EditorMode.rotate");


RotateEditorMode::RotateEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.rotate_btn"))))
{
}

RotateEditorMode::~RotateEditorMode()
{
}

const Id& RotateEditorMode::getId() const
{
    return id_;
}

} // namespace pbj

#endif
