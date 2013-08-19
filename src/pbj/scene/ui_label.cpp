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

UILabel::UILabel()
    : text_scale_(1.0f, 1.0f),
      align_(AlignLeft),
      text_transform_valid_(false)
{
}

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

void UILabel::setText(const std::string& text)
{
    text_ = text;
    text_transform_valid_ = false;
}

const std::string& UILabel::getText() const
{
    return text_;
}

void UILabel::setTextScale(const vec2& scale)
{
    if (scale != text_scale_)
    {
        text_scale_ = scale;
        text_transform_valid_ = false;
    }
}

const vec2& UILabel::getTextScale() const
{
    return text_scale_;
}

void UILabel::setTextColor(const color4& color)
{
    text_color_ = color;
}

const color4& UILabel::getTextColor() const
{
    return text_color_;
}

void UILabel::setFont(const gfx::TextureFont* font)
{
    if (font != font_)
    {
        font_ = font;
        text_transform_valid_ = false;
    }
}

const gfx::TextureFont* UILabel::getFont() const
{
    return font_;
}

void UILabel::setAlign(Align align)
{
    if (align != align_)
    {
        align_ = align;
        text_transform_valid_ = false;
    }
}

UILabel::Align UILabel::getAlign() const
{
    return align_;
}

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

void UILabel::onBoundsChange_()
{
    text_transform_valid_ = false;
}

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
