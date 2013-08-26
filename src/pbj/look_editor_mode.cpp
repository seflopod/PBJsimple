///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/look_editor_mode.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::LookEditorMode functions.

#ifdef PBJ_EDITOR

#include "pbj/look_editor_mode.h"
#include "pbj/editor.h"

#include <iostream>

namespace pbj {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Identifies this editor mode and allows the safe casting of an
///         EditorMode pointer or ref to a child type.
Id LookEditorMode::id_("EditorMode.look");

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a LookEditorMode object.
///
/// \details The construction of an EditorMode object signals that the Editor
///         has entered that mode.
///
/// \param  editor The editor object.
LookEditorMode::LookEditorMode(Editor& editor)
    : EditorMode(editor, static_cast<scene::UIButton*>(editor.getUIElement(Id("menu.look_btn"))))
{
    editor_.getUIElement(Id("menu.look_panel"))->setVisible(true);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destroys a LookEditorMode object.
///
/// \details The destruction of an EditorMode object signals that the Editor
///         is leaving that mode, so any active edits must be finalized or
///         cancelled.
LookEditorMode::~LookEditorMode()
{
    for (U32 button = 0; button < 3; ++button)
    {
        if (btn_down_[button])
            onDragUpdate(button, btn_down_pos_[button], btn_down_pos_[button]);
    }

    editor_.getUIElement(Id("menu.look_panel"))->setVisible(false);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns this EditorMode's Id.
///
/// \brief  The ID differentiates the editor mode and allows the safe casting
///         of an EditorMode pointer or ref to a child type.
///
/// \return The id_ static member.
const Id& LookEditorMode::getId() const
{
    return id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse button is pressed while not over a UI element
///         and in this editor mode.
///
/// \details When the left button is pressed, saves the current camera position
///         so that it can be updated in onDragUpdate()
///
/// \param  button The button that was pressed.
/// \param  position The mouse's position at the time of the event.
void LookEditorMode::onMouseDown(I32 button, const vec2& position)
{
    EditorMode::onMouseDown(button, position);

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        drag_start_camera_pos_ = editor_.getCamera().getTargetPosition();
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when the mouse is moved while a particular mouse button is
///         pressed.
///
/// \details If the left button is pressed, sets the camera's target position
///         so that it is offset in the opposite direction as the mouse
///         was dragged.  This creates a sense of "pulling" the world,
///         rather than the camera.
///
/// \param  button The mouse button that is pressed.
///         start  The position of the mouse when the drag operation started.
///         end    The current position of the mouse.
void LookEditorMode::onDragUpdate(I32 button, const vec2& start, const vec2& end)
{
    EditorMode::onDragUpdate(button, start, end);

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        editor_.getCamera().setTargetPosition(drag_start_camera_pos_ + start - end);
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse button is clicked.
///
/// \details If the right button is pressed, set the camera's target position
///         to the mouse position.
///
/// \param  button The button that was clicked.
/// \param  start  The mouse's position when the click began
/// \param  end    The mouse's position when the click ended  (should be the
///                same as start.
void LookEditorMode::onClick(I32 button, const vec2& start, const vec2& end)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        editor_.getCamera().setTargetPosition(end);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when the mouse wheel is scrolled when the mouse is not over
///         a UI element.
///
/// \details Multiplies or divides the current zoom level by 4/5 depending
///         on the direction of the wheel movement.
///
/// \param  delta Indicates the direction in which the wheel moved.
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
