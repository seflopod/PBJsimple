#ifdef PBJ_EDITOR

#include "pbj/clobber_editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

Id ClobberEditorMode::id_("EditorMode.clobber");


ClobberEditorMode::ClobberEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.clobber_btn"))))
{
}

ClobberEditorMode::~ClobberEditorMode()
{
}

const Id& ClobberEditorMode::getId() const
{
    return id_;
}

} // namespace pbj

#endif
