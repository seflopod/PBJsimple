///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_styles.h
/// \author Benjamin Crist
///
/// \brief  UI style struct header.

#ifndef PBJ_SCENE_UI_STYLES_H_
#define PBJ_SCENE_UI_STYLES_H_

#include "pbj/gfx/texture_font.h"
#include "pbj/_pbj.h"
#include "pbj/_math.h"

namespace pbj {
namespace scene {

///////////////////////////////////////////////////////////////////////////////
/// \brief  A UIPanelStyle determines the appearance of a rectangular UI
///         element (primarily, UIPanels).
///
/// \details Additionally, it is used to define
///         the background of UIButtons in UIButtonStyle.
///
/// \author Ben Crist
struct UIPanelStyle
{
    color4 background_color_top;    ///< The color of the inside of the button at the top
    color4 background_color_bottom; ///< The color of the inside of the button at the bottom
    color4 border_color;            ///< The color of the border of the button
    color4 margin_color;            ///< The color of the margins of the button

    F32 margin_left;                ///< The distance between the left edge of the button and the middle of the left border.
    F32 margin_right;
    F32 margin_top;
    F32 margin_bottom;

    F32 border_width_left;          ///< The distance between the center of the border and the edge of the border. (should actually be called 'half_width')
    F32 border_width_right;
    F32 border_width_top;
    F32 border_width_bottom;
};

///////////////////////////////////////////////////////////////////////////////
/// \brief  A UIPanelStyle determines the appearance of a UIButton element.
///
/// \details In addition to the panel style, determines the text color, font,
///         and text scale used for the button text.  The text is always
///         drawn in the center of the button's background area however, the
///         only way to adjust its alignment is to change the border/margin
///         widths in the panel style portion.
///
/// \author Ben Crist
struct UIButtonStyle
{
    const gfx::TextureFont* font;
    color4 text_color;
    vec2 text_scale;
    UIPanelStyle panel;
};

UIPanelStyle loadUIPanelStyle(sw::Sandwich& sandwich, const Id& id);
UIButtonStyle loadUIButtonStyle(sw::Sandwich& sandwich, const Id& id, sw::ResourceManager& rm);

} // namespace pbj::scene
} // namespace pbj

#endif
