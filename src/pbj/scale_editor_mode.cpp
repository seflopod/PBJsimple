#ifdef PBJ_EDITOR

#include "pbj/scale_editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

Id ScaleEditorMode::id_("EditorMode.scale");


ScaleEditorMode::ScaleEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.scale_btn"))))
{
}

ScaleEditorMode::~ScaleEditorMode()
{
}

const Id& ScaleEditorMode::getId() const
{
    return id_;
}

} // namespace pbj

#endif