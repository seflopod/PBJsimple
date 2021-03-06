///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_button.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::scene::UIButton functions.

#include "pbj/scene/ui_button.h"

#include "pbj/engine.h"
#include "pbj/_math.h"

namespace pbj {
namespace scene {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new UIButton.
///
/// \details The button defaults to the "std_btn" state UIButtonStyle ids.
UIButton::UIButton()
    : active_style_(nullptr),
      disabled_(false),
      active_(false),
      kbd_active_(false),
      hovered_(false)
{
    style_ids_[SNormal]         = sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.normal"));
    style_ids_[SHovered]        = sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.hovered"));
    style_ids_[SActive]         = sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.active"));
    style_ids_[SFocused]        = sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.focused"));
    style_ids_[SFocusedHovered] = sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.focused_hovered"));
    style_ids_[SFocusedActive]  = sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.focused_active"));
    style_ids_[SDisabled]       = sw::ResourceId(Id(PBJ_ID_PBJBASE), Id("std_btn.disabled"));

    label_.setAlign(UILabel::AlignCenter);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destructor is necessary for polymorphism, but does nothing.
UIButton::~UIButton()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the text which appears on the button.
void UIButton::setText(const std::string& text)
{
    label_.setText(text);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the button's current label text.
const std::string& UIButton::getText() const
{
    return label_.getText();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the callback function which will be called when the button is
///         clicked.
void UIButton::setClickCallback(const std::function<void()>& callback)
{
    callback_ = callback;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the Id of a UIButtonStyle to use for the specified button
///         state.
void UIButton::setStyle(State state, const sw::ResourceId& button_style)
{
    style_ids_[state] = button_style;

    if (getCurrentState_() == state)
        active_style_ = nullptr;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the UIButtonStyle being used for the requested button
///         state.
const sw::ResourceId& UIButton::getStyle(State state) const
{
    return style_ids_[state];
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Draws the button.
void UIButton::draw()
{
    if (isVisible() && view_)
    {
        refreshConfig_();

        glLoadMatrixf(glm::value_ptr(btn_transform_));
        gfx::Texture::disable();

        glBegin(GL_QUADS);
            glColor4fv(glm::value_ptr(active_style_->panel.background_color_top));
            glVertex2f(border_bounds_[1].x, border_bounds_[0].y);
            glVertex2fv(glm::value_ptr(border_bounds_[0]));
            glColor4fv(glm::value_ptr(active_style_->panel.background_color_bottom));
            glVertex2f(border_bounds_[0].x, border_bounds_[1].y);
            glVertex2fv(glm::value_ptr(border_bounds_[1]));
        glEnd();

        glColor4fv(glm::value_ptr(active_style_->panel.border_color));
        glBegin(GL_TRIANGLE_STRIP);
            glVertex2f(border_bounds_[3].x, border_bounds_[2].y);
            glVertex2f(border_bounds_[1].x, border_bounds_[0].y);
            glVertex2f(border_bounds_[3].x, border_bounds_[3].y);
            glVertex2f(border_bounds_[1].x, border_bounds_[1].y);
            glVertex2f(border_bounds_[2].x, border_bounds_[3].y);
            glVertex2f(border_bounds_[0].x, border_bounds_[1].y);
            glVertex2f(border_bounds_[2].x, border_bounds_[2].y);
            glVertex2f(border_bounds_[0].x, border_bounds_[0].y);
            glVertex2f(border_bounds_[3].x, border_bounds_[2].y);
            glVertex2f(border_bounds_[1].x, border_bounds_[0].y);
        glEnd();

        glColor4fv(glm::value_ptr(active_style_->panel.margin_color));
        glBegin(GL_TRIANGLE_STRIP);
            glVertex2f(1, 0);
            glVertex2f(border_bounds_[3].x, border_bounds_[2].y);
            glVertex2f(1, 1);
            glVertex2f(border_bounds_[3].x, border_bounds_[3].y);
            glVertex2f(0, 1);
            glVertex2f(border_bounds_[2].x, border_bounds_[3].y);
            glVertex2f(0, 0);
            glVertex2f(border_bounds_[2].x, border_bounds_[2].y);
            glVertex2f(1, 0);
            glVertex2f(border_bounds_[3].x, border_bounds_[2].y);
        glEnd();

        label_.draw();
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Makes the button disabled, preventing it from being clicked or
///         activated via the keyboard, and applying the disabled style.
void UIButton::setDisabled(bool disabled)
{
    disabled_ = disabled;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns true if this button is disabled.
bool UIButton::isDisabled() const
{
    return disabled_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns true if the button is currently being pressed.
bool UIButton::isActive() const
{
    return active_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns true if the mouse is currently over the button.
bool UIButton::isHovered() const
{
    return hovered_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns true if the button is focusable.
bool UIButton::isFocusable() const
{
    return isFullyVisible() && !isDisabled();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when the mouse moves into the element.  Sets the hovered_
///         flag.
///
/// \param  position The current mouse position in screen coordinates.
void UIButton::onMouseIn(const ivec2& position)
{
    hovered_ = true;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when the mouse moves out of the element.  Clears the
///         hovered_ flag.
///
/// \param  position The current mouse position in screen coordinates.
void UIButton::onMouseOut(const ivec2& position)
{
    hovered_ = false;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse button is pressed over this element.
void UIButton::onMouseDown(I32 button)
{
    UIElement::onMouseDown(button);
    if (button == GLFW_MOUSE_BUTTON_1)
        active_ = true;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse button is released over this element.
void UIButton::onMouseUp(I32 button)
{
    if (button == GLFW_MOUSE_BUTTON_1)
        active_ = false;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a mouse button is clicked over this element.
void UIButton::onMouseClick(I32 button)
{
    if (!disabled_ && button == GLFW_MOUSE_BUTTON_1 && callback_)
        callback_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a key is released while this element has focus.
void UIButton::onKeyUp(I32 keycode, I32 modifiers)
{
    if (kbd_active_ && ((modifiers & (GLFW_MOD_ALT | GLFW_MOD_CONTROL | GLFW_MOD_SHIFT | GLFW_MOD_SUPER)) == 0))
    {
        if (keycode == GLFW_KEY_SPACE || keycode == GLFW_KEY_ENTER || keycode == GLFW_KEY_KP_ENTER)
        {
            onMouseClick(GLFW_MOUSE_BUTTON_1);
            kbd_active_ = false;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when a key is pressed while this element has focus.
void UIButton::onKeyPressed(I32 keycode, I32 modifiers)
{
    UIElement::onKeyPressed(keycode, modifiers);
    if (isFocused() && ((modifiers & (GLFW_MOD_ALT | GLFW_MOD_CONTROL | GLFW_MOD_SHIFT | GLFW_MOD_SUPER)) == 0))
    {
        if (keycode == GLFW_KEY_SPACE || keycode == GLFW_KEY_ENTER || keycode == GLFW_KEY_KP_ENTER)
        {
            kbd_active_ = true;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when the button's position or dimensions have changed.
void UIButton::onBoundsChange_()
{
    active_style_ = nullptr;
    label_.view_ = view_;
    label_.inv_view_ = inv_view_;
    label_.focused_element_ = focused_element_;
    label_.onBoundsChange_();
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when the button gains or loses focus.
void UIButton::onFocusChange_()
{
    if (!isFocused())
        kbd_active_ = false;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the current state the button is in.
UIButton::State UIButton::getCurrentState_() const
{
    if (isDisabled())
        return SDisabled;

    if (isFocused())
    {
        if ((hovered_ && active_) || kbd_active_)
            return SFocusedActive;

        if (hovered_)
            return SFocusedHovered;

        return SFocused;
    }
    else
    {
        if (hovered_)
        {
            if (active_)
                return SActive;

            return SHovered;
        }

        return SNormal;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  ensures that the button is displaying correctly for the state the
///         button is in.
void UIButton::refreshConfig_()
{
    const UIButtonStyle& style = getEngine().getResourceManager().getUIButtonStyle(
        style_ids_[getCurrentState_()]);

    if (active_style_ != &style)
    {
        if (!view_)
            return;

        active_style_ = &style;

        label_.setFont(style.font);
        label_.setTextScale(style.text_scale);
        label_.setTextColor(style.text_color);

        label_.setPosition(getPosition() + vec2(I32(style.panel.margin_left + style.panel.border_width_left), I32(style.panel.margin_top + style.panel.border_width_top)));
        label_.setDimensions(getDimensions() - vec2(I32(style.panel.margin_left + style.panel.border_width_left + style.panel.border_width_right + style.panel.margin_right),
                                                    I32(style.panel.margin_top + style.panel.border_width_top + style.panel.border_width_bottom + style.panel.margin_bottom)));

        vec3 scale(getDimensions(), 1);
        vec3 translate(getPosition(), 0);
        btn_transform_ = glm::scale(glm::translate(*view_, translate), scale);

        vec2 inv_scale(1.0f / scale.x, 1.0f / scale.y);

        border_bounds_[0] = inv_scale * vec2(style.panel.margin_left + style.panel.border_width_left, style.panel.margin_top + style.panel.border_width_top);
        border_bounds_[1] = vec2(1, 1) - inv_scale * vec2(style.panel.margin_right + style.panel.border_width_right, style.panel.margin_bottom + style.panel.border_width_bottom);

        border_bounds_[2] = inv_scale * vec2(style.panel.margin_left - style.panel.border_width_left, style.panel.margin_top - style.panel.border_width_top);
        border_bounds_[3] = vec2(1, 1) - inv_scale * vec2(style.panel.margin_right - style.panel.border_width_right, style.panel.margin_bottom - style.panel.border_width_bottom);
    }
}

} // namespace pbj::scene
} // namespace pbj
