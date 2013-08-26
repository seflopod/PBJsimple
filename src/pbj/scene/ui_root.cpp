///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_root.cpp
/// \author Ben Crist

#include "pbj/scene/ui_root.h"

#include "pbj/engine.h"
#include "pbj/window.h"
#include "pbj/input_controller.h"

#include <cassert>
#include <iostream>

namespace pbj {
namespace scene {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new UIRoot.
///
/// \details Input handler functions are registered with InputController and
///         the panel member is set up.
UIRoot::UIRoot()
    : under_mouse_(nullptr),
      focused_element_(&panel),
      button1_down_over_(nullptr),
      button2_down_over_(nullptr),
      button3_down_over_(nullptr),
      order_index_offset_(1000),
      input_handlers_active_(true)
{
    mouse_motion_listener_id_ = InputController::registerMouseMotionListener(
        [&](F64 x, F64 y)
        {
            if (input_handlers_active_)
                onMouseMove(ivec2(x, y));
        }
    );

    mouse_button_any_listener_id_ = InputController::registerMouseButtonAnyListener(
        [&](I32 button, I32 action, I32 mods)
        {
            if (input_handlers_active_)
                onMouseButton(button, action == GLFW_PRESS);
        }
    );

    key_all_listener_id_ = InputController::registerKeyAllListener(
        [&](I32 keycode, I32 scancode, I32 action, I32 modifiers)
        {
            if (input_handlers_active_)
                onKey(keycode, action, modifiers);
        }
    );

    char_input_listener_id_ = InputController::registerCharInputListener(
        [&](U32 codepoint)
        {
            if (input_handlers_active_)
                onCharacter(codepoint);
        }
    );


    Window* window = getEngine().getWindow();

    context_resize_listener_id_ = window->registerContextResizeListener(
        [&](I32 width, I32 height)
        {
            panel.setPosition(vec2());
            panel.setDimensions(vec2(width, height));
            projection_matrix_ = glm::ortho(0.0f, F32(width), F32(height), 0.0f);
            panel.onBoundsChange_();
        }
    );

    panel.view_ = &view_matrix_;
    panel.inv_view_ = &view_matrix_;
    panel.focused_element_ = &focused_element_;
    panel.setPosition(vec2());
    panel.setDimensions(vec2(window->getContextSize()));
    projection_matrix_ = glm::ortho(0.0f, panel.getDimensions().x, panel.getDimensions().y, 0.0f);
    panel.onBoundsChange_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destroys the UIRoot.
///
/// \details The input handlers registered in the constructor are released.
UIRoot::~UIRoot()
{
    InputController::cancelMouseMotionListener(mouse_motion_listener_id_);
    InputController::cancelMouseButtonAnyListener(mouse_button_any_listener_id_);
    InputController::cancelKeyAllListener(key_all_listener_id_);
    InputController::cancelCharInputListener(char_input_listener_id_);
    getEngine().getWindow()->cancelContextResizeListener(context_resize_listener_id_);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Draws the UI hierarchy.
///
/// \details The GL_PROJECTION and GL_MODELVIEW matrices are modified during
///         the draw and the UIRoot does not restore them to their prior
///         values.  Therefore they must be reset every frame! (UI is usually
///         the last thing drawn on each frame)
void UIRoot::draw()
{
    Window* window = getEngine().getWindow();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection_matrix_));
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    panel.draw();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  If the UIRoot's panel is visible, it becomes focused.  Otherwise,
///         the focus is cleared (focused_element_ = nullptr).
void UIRoot::clearFocus()
{
    if (panel.isVisible())
        panel.setFocused();
    else
    {
        UIElement* old_focus = focused_element_;
        focused_element_ = nullptr;
        if (old_focus)
            old_focus->onFocusChange_();
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the currently focused element, or nullptr if there is no
///         focus.
UIElement* UIRoot::getFocus()
{
    return focused_element_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the UIElement that was under the mouse the last time it
///         moved or nullptr if there is no such element.
UIElement* UIRoot::getElementUnderMouse()
{
    return under_mouse_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Enables or disables the input handler functions.  This acts as a
///         global enable/disable for the entire UI hierarchy.
///
/// \param  enabled true if the input handlers should be active.
void UIRoot::setInputEnabled(bool enabled)
{
    input_handlers_active_ = enabled;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns true if the input handler functions are active.
bool UIRoot::isInputEnabled() const
{
    return input_handlers_active_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when the mouse movement handler is fired.
void UIRoot::onMouseMove(const ivec2& position)
{
    mouse_position_ = position;
    UIElement* under_mouse = panel.getElementAt(position);

    if (under_mouse != under_mouse_)
    {
        if (under_mouse_)
            under_mouse_->onMouseOut(position);

        if (under_mouse)
            under_mouse->onMouseIn(position);

        under_mouse_ = under_mouse;
    }

    if (under_mouse_)
        under_mouse_->onMouseMove(position);

    if (button1_down_over_ && button1_down_over_ != under_mouse_)
        button1_down_over_->onMouseMove(position);

    if (button2_down_over_ && button2_down_over_ != under_mouse_ && button2_down_over_ != button1_down_over_)
        button2_down_over_->onMouseMove(position);

    if (button3_down_over_ && button3_down_over_ != under_mouse_ && button3_down_over_ != button1_down_over_ && button3_down_over_ != button2_down_over_)
        button3_down_over_->onMouseMove(position);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse button is pressed or released.
void UIRoot::onMouseButton(I32 button, bool down)
{
    UIElement* under_mouse = panel.getElementAt(mouse_position_);

    //PBJ_LOG(VInfo) << "under mouse: " << under_mouse << PBJ_LOG_END;

    if (under_mouse)
    {
        if (down)
            under_mouse->onMouseDown(button);
        else
            under_mouse->onMouseUp(button);
    }

    UIElement** down_over = nullptr;
    if (button == GLFW_MOUSE_BUTTON_1)
        down_over = &button1_down_over_;
    else if (button == GLFW_MOUSE_BUTTON_2)
        down_over = &button2_down_over_;
    else if (button == GLFW_MOUSE_BUTTON_3)
        down_over = &button3_down_over_;

    if (down_over)
    {
        if (down)
        {
            *down_over = under_mouse;

            if (under_mouse && under_mouse->isFocusable())
                under_mouse->setFocused();
            else
                clearFocus();
        }
        else
        {
            // mouse up event for one of first 3 buttons

            if (*down_over)
            {
                if (*down_over != under_mouse)
                    (*down_over)->onMouseUp(button);
                else
                {
                    under_mouse->onMouseClick(button);

                    // TODO: dispatch double click event if 2 mouse clicks happen (same button) within ~350 ms.
                }

                *down_over = nullptr;
            }
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a key is pressed or released on the keyboard.
void UIRoot::onKey(I32 keycode, I32 action, I32 modifiers)
{
    if (!focused_element_ && panel.isVisible())
        panel.setFocused();

    if (!focused_element_)
        return;

    switch (action)
    {
        case GLFW_PRESS:
            focused_element_->onKeyDown(keycode, modifiers);
            focused_element_->onKeyPressed(keycode, modifiers);
            break;

        case GLFW_REPEAT:
            focused_element_->onKeyDown(keycode, modifiers);
            break;

        case GLFW_RELEASE:
            focused_element_->onKeyUp(keycode, modifiers);
            break;

        default:
            assert(false);  // no other action should ever be seen.
            break;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a character is input.  This does not necessarily map to
///         onKey() events in a 1:1 manner.
void UIRoot::onCharacter(I32 codepoint)
{
    if (!focused_element_)
        return;

    focused_element_->onCharacter(codepoint);
}

} // namespace pbj::scene
} // namespace pbj
