#ifdef PBJ_EDITOR

#include "pbj/move_editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

Id MoveEditorMode::id_("EditorMode.move");


MoveEditorMode::MoveEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.move_btn"))))
{
}

MoveEditorMode::~MoveEditorMode()
{
}

const Id& MoveEditorMode::getId() const
{
    return id_;
}

} // namespace pbj

#endif
