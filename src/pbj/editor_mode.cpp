///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/editor_mode.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::EditorMode functions.

#ifdef PBJ_EDITOR

#include "pbj/editor_mode.h"
#include "pbj/editor.h"

namespace pbj {
    
///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs an EditorMode object.
///
/// \details The construction of an EditorMode object signals that the Editor
///         has entered a particular mode.
///
/// \param  editor The editor object, which is stored for possible use by
///         derrived classes.
EditorMode::EditorMode(Editor& editor)
    : editor_(editor)
{
    for (U32 button = 0; button < 3; ++button)
    {
        btn_down_[button] = false;
        btn_down_moved_[button] = false;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs an EditorMode object, specifying a UIButton that should
///         be highlighed while the mode is active.
///
/// \details The construction of an EditorMode object signals that the Editor
///         has entered a particular mode.
///
/// \param  editor The editor object, which is stored for possible use by
///         derrived classes.
EditorMode::EditorMode(Editor& editor, scene::UIButton* btn)
    : editor_(editor),
      btn_(btn)
{
    for (U32 button = 0; button < 3; ++button)
    {
        btn_down_[button] = false;
        btn_down_moved_[button] = false;
    }
    if (btn_)
    {
        editor_.highlightUIButton(btn_);
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destroys an EditorMode object.
///
/// \details The destruction of an EditorMode object signals that the Editor
///         is leaving that mode, so any active edits must be finalized or
///         cancelled.
EditorMode::~EditorMode()
{
    if (btn_)
    {
        editor_.unhighlightUIButton(btn_);
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse button is pressed while not over a UI element
///         and in this editor mode.
///
/// \details Keeps track of the position where the mouse was pressed for
///         future use.  Overridden onMouseDown methods should be sure
///         to call EditorMode::onMouseDown().
///
/// \param  button The button that was pressed.
/// \param  position The mouse's position at the time of the event.
void EditorMode::onMouseDown(I32 button, const vec2& position)
{
    btn_down_pos_[button] = position;
    btn_down_[button] = true;
    btn_down_moved_[button] = false;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse button is released while not over a UI element
///         and in this editor mode.
///
/// \details When onMouseDown is called, the corresponding onMouseUp or
///         onMouseCancel event will always be called if the EditorMode stays
///         the same, even if the mouse moves over a UI element.
///
///         If the mouse has not moved since it was pressed, a click event
///         is generated.
///
///         Overridden functions should be sure to call EditorMode::onMouseUp.
///
/// \param  button The button that was released.
/// \param  position The mouse's position at the time of the event.
void EditorMode::onMouseUp(I32 button, const vec2& position)
{
    if (btn_down_[button])
    {
        if (btn_down_moved_[button] == false)
            onClick(button, btn_down_pos_[button], position);
        else
            onDragUpdate(button, btn_down_pos_[button], position);

        btn_down_[button] = false;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse press is cancelled, for example if it is
///         released over a UI element.
///
/// \details If a drag is in progress, it is ended by issuing an onDragUpdate
///         with the position the drag started from.
///
///         Overridden functions should be sure to call
///         EditorMode::onMouseCancel.
///
/// \param  button The button that was cancelled.
/// \param  position The mouse's position at the time of the event.
void EditorMode::onMouseCancel(I32 button, const vec2& position)
{
    if (btn_down_[button])
    {
        if (btn_down_moved_[button])
            onDragUpdate(button, btn_down_pos_[button], btn_down_pos_[button]);

        btn_down_[button] = false;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when the mouse is moved while at least one button is down.
///
/// \details onDragUpdate events are generated for any buttons that are
///         currently pressed.
///
///         Overridden functions should be sure to call
///         EditorMode::onMouseMove.
///
/// \param  position The mouse's position at the time of the event.
void EditorMode::onMouseMove(const vec2& position)
{
    for (U32 button = 0; button < 3; ++button)
    {
        if (btn_down_[button])
        {
            if (glm::length(position - btn_down_pos_[button]) > 1.0f)
            {
                btn_down_moved_[button] = true;
            }
            onDragUpdate(button, btn_down_pos_[button], position);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when the mouse wheel is scrolled when the mouse is not over
///         a UI element.
/// 
/// \param  delta Indicates the direction in which the wheel moved.
void EditorMode::onMouseWheel(I32 delta)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when the mouse is moved while a particular mouse button is
///         pressed.
///
/// \param  button The mouse button that is pressed.
///         start  The position of the mouse when the drag operation started.
///         end    The current position of the mouse.
void EditorMode::onDragUpdate(I32 button, const vec2& start, const vec2& end)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse button is clicked.
///
/// \param  button The button that was clicked.
/// \param  start  The mouse's position when the click began
/// \param  end    The mouse's position when the click ended  (should be the
///                same as start.
void EditorMode::onClick(I32 button, const vec2& start, const vec2& end)
{
}

} // namespace pbj

#endif
