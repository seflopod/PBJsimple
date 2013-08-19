#include "pbj/mimic_editor_mode.h"

#include "pbj/editor.h"

namespace pbj {

Id MimicEditorMode::id_("EditorMode.mimic");


MimicEditorMode::MimicEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.mimic_btn"))))
{
}

MimicEditorMode::~MimicEditorMode()
{
}

const Id& MimicEditorMode::getId() const
{
    return id_;
}

} // namespace pbj
