#ifdef PBJ_EDITOR

#include "pbj/look_editor_mode.h"
#include "pbj/editor.h"

#include <iostream>

namespace pbj {

Id LookEditorMode::id_("EditorMode.look");


LookEditorMode::LookEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.look_btn"))))
{
    editor_.getUIElement(Id("menu.look_panel"))->setVisible(true);
}

LookEditorMode::~LookEditorMode()
{
    for (U32 button = 0; button < 3; ++button)
    {
        if (btn_down_[button])
            onDragUpdate(button, btn_down_pos_[button], btn_down_pos_[button]);
    }

    editor_.getUIElement(Id("menu.look_panel"))->setVisible(false);
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
        editor_.getCamera().setTargetPosition(drag_start_camera_pos_ + start - end);
    }
}

void LookEditorMode::onClick(I32 button, const vec2& start, const vec2& end)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        //PBJ_LOG(VInfo) << "Move Click" << PBJ_LOG_END;
        editor_.getCamera().setTargetPosition(end);
    }
}

void LookEditorMode::onMouseWheel(I32 delta)
{
    F64 zoom = editor_.getZoom();

    while (delta > 0)
    {
        zoom *= 4.0 / 5.0;
        --delta;
    }
    while (delta < 0)
    {
        zoom *= 5.0 / 4.0;
        ++delta;
    }

    editor_.setZoom(zoom);
}

} // namespace pbj

#endif
