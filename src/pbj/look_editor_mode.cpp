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

void LookEditorMode::onMouseDown(I32 button, const vec2& position)
{
    EditorMode::onMouseDown(button, position);

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        drag_start_camera_pos_ = editor_.getCamera().getTargetPosition();
    }
}

void LookEditorMode::onDragUpdate(I32 button, const vec2& start, const vec2& end)
{
    EditorMode::onDragUpdate(button, start, end);

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        editor_.getCamera().setTargetPosition(drag_start_camera_pos_ + end - start);
    }
}

void LookEditorMode::onClick(I32 button, const vec2& start, const vec2& end)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        editor_.getCamera().setTargetPosition(end);
    }
}

} // namespace pbj

#endif
