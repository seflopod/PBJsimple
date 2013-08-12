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

struct UIButtonStyle
{
    const gfx::TextureFont* font;
    color4 text_color;          ///< The color of the button text
    vec2 text_scale;
    UIPanelStyle panel;
};

UIPanelStyle loadUIPanelStyle(sw::Sandwich& sandwich, const Id& id);
UIButtonStyle loadUIButtonStyle(sw::Sandwich& sandwich, const Id& id, sw::ResourceManager& rm);

} // namespace pbj::scene
} // namespace pbj

#endif
