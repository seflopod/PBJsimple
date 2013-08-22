#ifdef PBJ_EDITOR

#include "pbj/rotate_editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

Id RotateEditorMode::id_("EditorMode.rotate");


RotateEditorMode::RotateEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.rotate_btn"))))
{
    editor_.getUIElement(Id("menu.rotate_panel"))->setVisible(true);
}

RotateEditorMode::~RotateEditorMode()
{
    editor_.getUIElement(Id("menu.rotate_panel"))->setVisible(false);
}

const Id& RotateEditorMode::getId() const
{
    return id_;
}

void RotateEditorMode::onMouseDown(I32 button, const vec2& position)
{
    EditorMode::onMouseDown(button, position);

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        onMouseCancel(GLFW_MOUSE_BUTTON_RIGHT, position);

        auto result = editor_.getClosestEntity(position, true, true);
        if (result.first && result.second / editor_.getZoom() < 8)
        {
            active_entity_ = result.first;
            original_rotation_ = active_entity_->getTransform().getRotation();
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        onMouseCancel(GLFW_MOUSE_BUTTON_LEFT, position);

        auto result = editor_.getClosestEntity(position, true, true);
        if (result.first && result.second / editor_.getZoom() < 8)
        {
            active_entity_ = result.first;
            original_rotation_ = active_entity_->getTransform().getRotation();
        }
    }
}

void RotateEditorMode::onMouseUp(I32 button, const vec2& position)
{
    EditorMode::onMouseUp(button, position);
    
    if (active_entity_ && (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT))
        active_entity_ = nullptr;
}

void RotateEditorMode::onMouseCancel(I32 button, const vec2& position)
{
    EditorMode::onMouseCancel(button, position);

    if (active_entity_ && (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT))
        active_entity_ = nullptr;
}

void RotateEditorMode::onDragUpdate(I32 button, const vec2& start, const vec2& end)
{
    EditorMode::onDragUpdate(button, start, end);

    if (active_entity_ && (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT))
    {
        vec2 delta = end - start;
        active_entity_->getTransform().setRotation(original_rotation_ + F32(3 * (delta.x + delta.y) / editor_.getZoom()));
    }
}

} // namespace pbj

#endif
