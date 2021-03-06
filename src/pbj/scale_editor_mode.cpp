///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scale_editor_mode.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::ScaleEditorMode functions.

#ifdef PBJ_EDITOR

#include "pbj/scale_editor_mode.h"
#include "pbj/editor.h"

namespace pbj {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Identifies this editor mode and allows the safe casting of an
///         EditorMode pointer or ref to a child type.
Id ScaleEditorMode::id_("EditorMode.scale");

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a ScaleEditorMode object.
///
/// \details The construction of an EditorMode object signals that the Editor
///         has entered that mode.
///
/// \param  editor The editor object.
ScaleEditorMode::ScaleEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.scale_btn"))))
{
    editor_.getUIElement(Id("menu.scale_panel"))->setVisible(true);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destroys a ScaleEditorMode object.
///
/// \details The destruction of an EditorMode object signals that the Editor
///         is leaving that mode, so any active edits must be finalized or
///         cancelled.
ScaleEditorMode::~ScaleEditorMode()
{
    editor_.getUIElement(Id("menu.scale_panel"))->setVisible(false);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns this EditorMode's Id.
///
/// \brief  The ID differentiates the editor mode and allows the safe casting
///         of an EditorMode pointer or ref to a child type.
///
/// \return The id_ static member.
const Id& ScaleEditorMode::getId() const
{
    return id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse button is pressed while not over a UI element
///         and in this editor mode.
///
/// \details Selects the closest entity within a reasonable range to be
///         acted upon.
///
///         If the other mouse button (L or R) is already pressed, a
///         cancel event is generated for that button.
///
/// \param  button The button that was pressed.
/// \param  position The mouse's position at the time of the event.
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

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse button is released while not over a UI element
///         and in this editor mode.
///
/// \details Resets the entity to be acted upon so that movements after the
///         mouse is released do not move any entity.
///
/// \param  button The button that was released.
/// \param  position The mouse's position at the time of the event.
void ScaleEditorMode::onMouseUp(I32 button, const vec2& position)
{
    EditorMode::onMouseUp(button, position);
    
    if (active_entity_ && (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT))
        active_entity_ = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse press is cancelled, for example if it is
///         released over a UI element.
///
/// \details Resets the entity to be acted upon so that movements after the
///         mouse is released do not move any entity.
///
/// \param  button The button that was cancelled.
/// \param  position The mouse's position at the time of the event.
void ScaleEditorMode::onMouseCancel(I32 button, const vec2& position)
{
    EditorMode::onMouseCancel(button, position);

    if (active_entity_ && (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT))
        active_entity_ = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when the mouse is moved while a particular mouse button is
///         pressed.
///
/// \details Updates the active object's scale relative to start position of
///         the drag.
///
///         Right and left mouse button dragging both have the same effect.
///         Dragging left or right decreases or increases (respectively) the
///         entity's scale on it's local x-axis.  Dragging up and down
///         increases or decreases (respectively) its scale on the local y-axis.
///
/// \param  button The mouse button that is pressed.
///         start  The position of the mouse when the drag operation started.
///         end    The current position of the mouse.}
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