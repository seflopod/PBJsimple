///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_label.h
/// \author Benjamin Crist
///
/// \brief  pbj::scene::UILabel class header.

#ifndef PBJ_SCENE_UI_LABEL_H_
#define PBJ_SCENE_UI_LABEL_H_

#include "pbj/scene/ui_element.h"
#include "pbj/gfx/texture_font.h"

namespace pbj {
namespace scene {

class UIButton;

///////////////////////////////////////////////////////////////////////////////
/// \brief  Defines a box with text drawn in it.
///
/// \details The text is anchored at either the left, center or right edge of
///         the label.  If the label's text is larger than the label, it will
///         overflow off the opposite side(s) from where it is anchored.
///         The label uses the font's Cap Height parameter to estimate the
///         vertical dimensions of the font and attempt to center the text
///         vertically.
///
// \author Ben Crist
class UILabel : public UIElement
{
    friend class UIButton;

public:
    enum Align
    {
        AlignLeft,
        AlignCenter,
        AlignRight,
    };

    UILabel();
    virtual ~UILabel();

    virtual UIElement* getElementAt(const ivec2& screen_position);

    void setText(const std::string& text);
    const std::string& getText() const;

    void setTextScale(const vec2& scale);
    const vec2& getTextScale() const;

    void setTextColor(const color4& color);
    const color4& getTextColor() const;

    void setFont(const gfx::TextureFont* font);
    const gfx::TextureFont* getFont() const;

    void setAlign(Align align);
    Align getAlign() const;

    virtual void draw();

private:
    virtual void onBoundsChange_();

    void calculateTextTransform_();

    const gfx::TextureFont* font_;
    std::string text_;
    color4 text_color_;
    vec2 text_scale_;
    Align align_;

    mat4 text_transform_;
    bool text_transform_valid_;
    
    // prevent copying/assignment
    UILabel(const UILabel&);
    void operator=(const UILabel&);
};

} // namespace pbj::scene
} // namespace pbj

#endif
