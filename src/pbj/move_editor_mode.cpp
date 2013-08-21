#ifdef PBJ_EDITOR

#include "pbj/move_editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

Id MoveEditorMode::id_("EditorMode.move");


MoveEditorMode::MoveEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.move_btn")))),
      moving_entity_(nullptr)
{
     editor_.getUIElement(Id("menu.move_panel"))->setVisible(true);
}

MoveEditorMode::~MoveEditorMode()
{
    if (moving_entity_)
        onMouseCancel(GLFW_MOUSE_BUTTON_1, vec2());

     editor_.getUIElement(Id("menu.move_panel"))->setVisible(false);
}

const Id& MoveEditorMode::getId() const
{
    return id_;
}

void MoveEditorMode::onMouseDown(I32 button, const vec2& position)
{
    EditorMode::onMouseDown(button, position);

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        onMouseCancel(GLFW_MOUSE_BUTTON_RIGHT, position);

        auto result = editor_.getClosestEntity(position, true, true);
        if (result.first && result.second / editor_.getZoom() < 8)
        {
            moving_entity_ = result.first;
            original_position_ = moving_entity_->getTransform().getPosition();
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        onMouseCancel(GLFW_MOUSE_BUTTON_LEFT, position);

        auto result = editor_.getClosestEntity(position, true, true);
        if (result.first && result.second / editor_.getZoom() < 8)
        {
            moving_entity_ = result.first;
            original_position_ = moving_entity_->getTransform().getPosition();
        }
    }
}

void MoveEditorMode::onMouseUp(I32 button, const vec2& position)
{
    EditorMode::onMouseUp(button, position);
    
    if (moving_entity_ && (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT))
        moving_entity_ = nullptr;
}

void MoveEditorMode::onMouseCancel(I32 button, const vec2& position)
{
    EditorMode::onMouseCancel(button, position);

    if (moving_entity_ && (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT))
        moving_entity_ = nullptr;
}

void MoveEditorMode::onDragUpdate(I32 button, const vec2& start, const vec2& end)
{
    EditorMode::onDragUpdate(button, start, end);

    if (moving_entity_ && (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT))
    {
        moving_entity_->getTransform().setPosition(original_position_ + end - start);
    }
}

} // namespace pbj

#endif
