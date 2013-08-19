#ifdef PBJ_EDITOR

#include "pbj/decorate_editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

Id DecorateEditorMode::id_("EditorMode.decorate");


DecorateEditorMode::DecorateEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.decorate_btn"))))
{
}

DecorateEditorMode::~DecorateEditorMode()
{
}

const Id& DecorateEditorMode::getId() const
{
    return id_;
}

} // namespace pbj

#endif
