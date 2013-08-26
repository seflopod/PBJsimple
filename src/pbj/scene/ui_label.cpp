///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_label.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::scene::UILabel functions.

#include "pbj/scene/ui_label.h"

#include "pbj/_math.h"

#include <iostream>

namespace pbj {
namespace scene {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new UILabel.
///
/// \details Text scale defaults to 1.0x1.0, Left alignment.
UILabel::UILabel()
    : text_scale_(1.0f, 1.0f),
      align_(AlignLeft),
      text_transform_valid_(false)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destructor is necessary for polymorphism, but does nothing.
UILabel::~UILabel()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Determines if the point at the specified position resides within
///         this element (or one of its children).
///
/// \param  position The screen coordinates of the point in question.
/// \return A pointer to an element at these coordinates, or nullptr if there
///         is no such element.
UIElement* UILabel::getElementAt(const ivec2& screen_position)
{
    return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the label's text to the specified string.
///
/// \param The new text to display on the label.
void UILabel::setText(const std::string& text)
{
    text_ = text;
    text_transform_valid_ = false;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the label's current text message.
const std::string& UILabel::getText() const
{
    return text_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the label's text scale.
void UILabel::setTextScale(const vec2& scale)
{
    if (scale != text_scale_)
    {
        text_scale_ = scale;
        text_transform_valid_ = false;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Returns the label's current text scale.
const vec2& UILabel::getTextScale() const
{
    return text_scale_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the label's text color.
void UILabel::setTextColor(const color4& color)
{
    text_color_ = color;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the label's current text color.
const color4& UILabel::getTextColor() const
{
    return text_color_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the label's current font.
void UILabel::setFont(const gfx::TextureFont* font)
{
    if (font != font_)
    {
        font_ = font;
        text_transform_valid_ = false;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the font object currently used by this label.
const gfx::TextureFont* UILabel::getFont() const
{
    return font_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the alignment of the text in this label.
void UILabel::setAlign(Align align)
{
    if (align != align_)
    {
        align_ = align;
        text_transform_valid_ = false;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the alignment of the text in this label.
UILabel::Align UILabel::getAlign() const
{
    return align_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Draws this label.
void UILabel::draw()
{
    if (isVisible() && view_ && font_)
    {
        if (!text_transform_valid_)
            calculateTextTransform_();

        glLoadMatrixf(glm::value_ptr(text_transform_));

        font_->print(text_, text_color_);
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Called when the label's position or dimensions have changed.
void UILabel::onBoundsChange_()
{
    text_transform_valid_ = false;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Recalculates the data necessary to draw the label.
void UILabel::calculateTextTransform_()
{
    if (!(view_ && font_))
        return;

    vec2 extra_space(getDimensions());
    extra_space -= vec2(font_->calculateTextWidth(text_) * text_scale_.x, font_->getCapHeight() * text_scale_.y);

    extra_space *= 0.5f;

    I32 bottom_spacing = I32(extra_space.y);
    I32 left_spacing = I32(extra_space.x);

    if (align_ == AlignLeft)
        left_spacing = 0;

    if (align_ == AlignRight)
        left_spacing *= 2;

    vec3 translation(getPosition(), 0);
    translation.y += getDimensions().y - bottom_spacing;
    translation.x += left_spacing;

    text_transform_ = glm::scale(glm::translate(*view_, translation), vec3(text_scale_.x, -text_scale_.y, 1));
    text_transform_valid_ = true;
}

} // namespace pbj::scene
} // namespace pbj
