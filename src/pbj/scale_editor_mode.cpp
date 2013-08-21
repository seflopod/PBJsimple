#ifdef PBJ_EDITOR

#include "pbj/scale_editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

Id ScaleEditorMode::id_("EditorMode.scale");


ScaleEditorMode::ScaleEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.scale_btn"))))
{
    editor_.getUIElement(Id("menu.scale_panel"))->setVisible(true);
}

ScaleEditorMode::~ScaleEditorMode()
{
    editor_.getUIElement(Id("menu.scale_panel"))->setVisible(false);
}

const Id& ScaleEditorMode::getId() const
{
    return id_;
}

void ScaleEditorMode::onMouseDown(I32 button, const vec2& position)
{
    EditorMode::onMouseDown(button, position);

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        onMouseCancel(GLFW_MOUSE_BUTTON_RIGHT, position);

        auto result = editor_.getClosestEntity(position, true, true);
        if (result.first && result.second / editor_.getZoom() < 8)
        {
            active_entity_ = result.first;
            original_scale_ = active_entity_->getTransform().getScale();
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        onMouseCancel(GLFW_MOUSE_BUTTON_LEFT, position);

        auto result = editor_.getClosestEntity(position, true, true);
        if (result.first && result.second / editor_.getZoom() < 8)
        {
            active_entity_ = result.first;
            original_scale_ = active_entity_->getTransform().getScale();
        }
    }
}

void ScaleEditorMode::onMouseUp(I32 button, const vec2& position)
{
    EditorMode::onMouseUp(button, position);
    
    if (active_entity_ && (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT))
        active_entity_ = nullptr;
}

void ScaleEditorMode::onMouseCancel(I32 button, const vec2& position)
{
    EditorMode::onMouseCancel(button, position);

    if (active_entity_ && (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT))
        active_entity_ = nullptr;
}

void ScaleEditorMode::onDragUpdate(I32 button, const vec2& start, const vec2& end)
{
    EditorMode::onDragUpdate(button, start, end);

    if (active_entity_ && (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT))
    {
        vec2 delta = end - start;
        delta /= editor_.getZoom() * 3.0f;

        vec2 new_scale = original_scale_;

        if (delta.x != 0)
            new_scale.x *= std::powf(1.33f, delta.x);

        if (delta.y != 0)
            new_scale.y *= std::powf(1.33f, delta.y);

        active_entity_->getTransform().setScale(new_scale);
    }
}

} // namespace pbj

#endif