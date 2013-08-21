#ifdef PBJ_EDITOR

#include "pbj/dupe_editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

Id DupeEditorMode::id_("EditorMode.dupe");


DupeEditorMode::DupeEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.dupe_btn"))))
{
}

DupeEditorMode::~DupeEditorMode()
{
}

const Id& DupeEditorMode::getId() const
{
    return id_;
}

} // namespace pbj

#endif
