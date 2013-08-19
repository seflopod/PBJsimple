#ifdef PBJ_EDITOR

#include "pbj/look_editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

Id LookEditorMode::id_("EditorMode.look");


LookEditorMode::LookEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.look_btn"))))
{
}

LookEditorMode::~LookEditorMode()
{
}

const Id& LookEditorMode::getId() const
{
    return id_;
}

} // namespace pbj

#endif
