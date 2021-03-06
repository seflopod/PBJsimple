///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_panel.cpp
/// \author Ben Crist

#include "pbj/scene/ui_panel.h"

#include "pbj/gfx/texture.h"

#include <iostream>

namespace pbj {
namespace scene {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a new UIPanel.  By default, it will not have a
///         PanelStyle.
UIPanel::UIPanel()
    : scale_(1.0f, 1.0f),
      style_(nullptr),
      panel_transform_valid_(false)
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destructor is necessary for polymorphism, but does nothing.
UIPanel::~UIPanel()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Sets the ResourceId of the UIPanelStyle to use for this panel.
void UIPanel::setStyle(const sw::ResourceId& panel_style)
{
    if (style_id_ != panel_style)
    {
        style_id_ = panel_style;
        style_ = nullptr;
        panel_transform_valid_ = false;
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieve the ResourceId of the UIPanelStyle in use by this panel.
const sw::ResourceId& UIPanel::getStyle() const
{
    return style_id_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Set the scale of the UIElement children of this panel.
void UIPanel::setScale(const vec2& scale)
{
    if (scale != scale_)
    {
        scale_ = scale;
        onBoundsChange_();
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the scale of this panel's children.
const vec2& UIPanel::getScale() const
{
    return scale_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Adds a new UIElement as a child of this panel.
///
/// \note   There is currently no way to remove an elemtn after it has been
///         added without destroying the UIRoot at the base of the hierarchy.
///         Instead you must set unused elements to be invisible.
void UIPanel::addElement(std::unique_ptr<UIElement>&& element)
{
    element->view_ = &view_matrix_;
    element->inv_view_ = &inv_view_matrix_;
    element->focused_element_ = focused_element_;
    element->parent_visible_ = &fully_visible_;
    element->onBoundsChange_();

    elements_.push_back(std::move(element));
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the first element which exists at the specified position.
///
/// \details Invisible elements don't count.
UIElement* UIPanel::getElementAt(const ivec2& screen_position)
{
    if (!inv_view_ || !isVisible())
        return nullptr;

    for (std::unique_ptr<UIElement>& ptr : elements_)
    {
        UIElement* element = ptr->getElementAt(screen_position);
        if (element)
            return element;
    }

    if (!panel_transform_valid_)
        calculateTransform_();

    if (style_)
    {
        vec2 pos = vec2(*inv_view_ * vec4(screen_position, 0, 1));

        if (pos.x >= position_.x && pos.x < position_.x + dimensions_.x &&
            pos.y >= position_.y && pos.y < position_.y + dimensions_.y)
        {
            return this;
        }
    }
    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief  Draws this UIPanel and all of its children.
void UIPanel::draw()
{
    if (!isVisible())
        return;

    if (!panel_transform_valid_)
            calculateTransform_();

    if (style_ && view_)
    {
        glLoadMatrixf(glm::value_ptr(panel_transform_));
        gfx::Texture::disable();

        glBegin(GL_QUADS);
            glColor4fv(glm::value_ptr(style_->background_color_top));
            glVertex2f(border_bounds_[1].x, border_bounds_[0].y);
            glVertex2fv(glm::value_ptr(border_bounds_[0]));
            glColor4fv(glm::value_ptr(style_->background_color_bottom));
            glVertex2f(border_bounds_[0].x, border_bounds_[1].y);
            glVertex2fv(glm::value_ptr(border_bounds_[1]));
        glEnd();

        glColor4fv(glm::value_ptr(style_->border_color));
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

        glColor4fv(glm::value_ptr(style_->margin_color));
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
    }

    for (std::unique_ptr<UIElement>& ptr : elements_)
        ptr->draw();
}

////////////////////////////////////////////////////////////////////////////////
/// \brief  Called when the UIPanel's position or dimensions have changed.
void UIPanel::onBoundsChange_()
{
    view_matrix_ = glm::scale(glm::translate(*view_, vec3(getPosition(), 0)), vec3(scale_, 1));
    inv_view_matrix_ = glm::inverse(view_matrix_);
    panel_transform_valid_ = false;

    for (std::unique_ptr<UIElement>& ptr : elements_)
    {
        ptr->view_ = &view_matrix_;
        ptr->inv_view_ = &inv_view_matrix_;
        ptr->focused_element_ = focused_element_;
        ptr->parent_visible_ = &fully_visible_;
        ptr->onBoundsChange_();
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \brief  called when the UIPanel's visibility or one of it's ancestor's
///         visibility has changed.
void UIPanel::onVisibilityChange_()
{
    UIElement::onVisibilityChange_();
    fully_visible_ = isFullyVisible();

    for (std::unique_ptr<UIElement>& ptr : elements_)
    {
        ptr->onVisibilityChange_();
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \brief  Recalculates the modelview matrix required to draw this panel's
///         background.
void UIPanel::calculateTransform_()
{
    if (!view_)
        return;

    if (style_ == nullptr)
    {
        if (style_id_ != sw::ResourceId())
            style_ = &getEngine().getResourceManager().getUIPanelStyle(style_id_);
    }

    vec3 scale(getDimensions(), 1);
    vec3 translate(getPosition(), 0);
    panel_transform_ = glm::scale(glm::translate(*view_, translate), scale);

    if (style_)
    {
        vec2 inv_scale(1.0f / scale.x, 1.0f / scale.y);

        // inside top left
        border_bounds_[0] = inv_scale * vec2(style_->margin_left + style_->border_width_left,
                                             style_->margin_top + style_->border_width_top);

        // inside bottom right
        border_bounds_[1] = vec2(1, 1) - inv_scale * vec2(style_->margin_right + style_->border_width_right,
                                                          style_->margin_bottom + style_->border_width_bottom);

        // outside top left
        border_bounds_[2] = inv_scale * vec2(style_->margin_left - style_->border_width_left,
                                             style_->margin_top - style_->border_width_top);

        // outside bottom right
        border_bounds_[3] = vec2(1, 1) - inv_scale * vec2(style_->margin_right - style_->border_width_right,
                                                          style_->margin_bottom - style_->border_width_bottom);
    }
}

} // namespace pbj::scene
} // namespace pbj
