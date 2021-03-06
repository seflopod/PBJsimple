///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_button.h
/// \author Benjamin Crist
///
/// \brief  pbj::scene::UIButton class header.

#ifndef PBJ_SCENE_UI_BUTTON_H_
#define PBJ_SCENE_UI_BUTTON_H_

#include "pbj/scene/ui_label.h"
#include "pbj/scene/ui_styles.h"

#include <functional>

namespace pbj {
namespace scene {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Pushbutton UI element.
///
/// \details Each button can be in one of several states:  Disabled (visible,
///         but not user-interactable), Normal, Hovered (mouse is over element)
///         Active (is being pressed), Focused (has keyboard focus), Focused
///         and Hovered, or Focused and Active.  The current state determines
///         most of the aesthetics of the button.  Only the button text and
///         onClick event handler are not determined by the current state.
///         Each state is assigned a UIButtonStyle id which determines the
///         way the button should look when in that state.  These can be
///         changed at any time to (for instance) make toggle buttons, etc.
///
/// \author Ben Crist
class UIButton : public UIElement
{
public:
    UIButton();
    virtual ~UIButton();

    void setText(const std::string& text);
    const std::string& getText() const;

    void setClickCallback(const std::function<void()>& callback);
    
    ///////////////////////////////////////////////////////////////////////////
    /// \brief  Defines the possible states the button can be in.
    enum State
    {
        SNormal = 0,
        SHovered = 1,
        SActive = 2,
        SFocused = 3,
        SFocusedHovered = 4,
        SFocusedActive = 5,
        SDisabled = 6,
        STATE_MAX_VALUE = 6
    };

    void setStyle(State state, const sw::ResourceId& button_style);
    const sw::ResourceId& getStyle(State state) const;

    virtual void draw();

    void setDisabled(bool disabled);
    bool isDisabled() const;

    bool isActive() const;
    bool isHovered() const;

    virtual bool isFocusable() const;

    virtual void onMouseIn(const ivec2& position);
    virtual void onMouseOut(const ivec2& position);
    virtual void onMouseDown(I32 button);
    virtual void onMouseUp(I32 button);
    virtual void onMouseClick(I32 button);
    virtual void onKeyUp(I32 keycode, I32 modifiers);
    virtual void onKeyPressed(I32 keycode, I32 modifiers);

private:
    virtual void onBoundsChange_();
    virtual void onFocusChange_();

    void refreshConfig_();
    State getCurrentState_() const;

    UILabel label_;
    std::function<void()> callback_;

    const UIButtonStyle* active_style_;
    mat4 btn_transform_;
    vec2 border_bounds_[4];

    sw::ResourceId style_ids_[STATE_MAX_VALUE + 1];

    bool disabled_;
    bool active_;
    bool kbd_active_;
    bool hovered_;

    // prevent copying/assignment
    UIButton(const UIButton&);
    void operator=(const UIButton&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
