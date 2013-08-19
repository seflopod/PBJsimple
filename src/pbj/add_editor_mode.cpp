#include "pbj/add_editor_mode.h"

#include "pbj/editor.h"

namespace pbj {

Id AddEditorMode::id_("EditorMode.add");


AddEditorMode::AddEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.add_btn"))))
{
}

AddEditorMode::~AddEditorMode()
{
}

const Id& AddEditorMode::getId() const
{
    return id_;
}

} // namespace pbj
