///////////////////////////////////////////////////////////////////////////////
/// \file   main.cpp
/// \author Benjamin Crist

#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>
#include <glm/glm.hpp>

#include "be/id.h"

typedef glm::vec4 color4;
typedef glm::vec4 vec4;
typedef glm::vec2 vec2;
typedef float F32;
typedef int64_t I64;
typedef uint64_t U64;

struct PanelStyle
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

struct ButtonStyle
{
    color4 text_color;          ///< The color of the button text
    vec2 text_scale;
    PanelStyle panel;
};

///////////////////////////////////////////////////////////////////////////////
int rgba8888(const vec4& color)
{
    glm::ivec4 temp(color.r * 255, color.g * 255, color.b * 255, color.a * 255);
    temp.r = std::max(0, std::min(255, temp.r));
    temp.g = std::max(0, std::min(255, temp.g));
    temp.b = std::max(0, std::min(255, temp.b));
    temp.a = std::max(0, std::min(255, temp.a));

    return temp.r | (temp.g << 8) | (temp.b << 16) | (temp.a << 24);
}

///////////////////////////////////////////////////////////////////////////////
void output(const ButtonStyle& bs, const std::string& bs_name)
{
    static be::Id font_id("std_font");
    be::Id bs_id(bs_name);
    be::Id ps_id(bs_name + ".panel");


    std::cout << "-- " << bs_name << ".panel" << std::endl
              << "INSERT INTO sw_ui_panel_styles VALUES (" << (I64)ps_id.value()
              << "," << rgba8888(bs.panel.background_color_top)
              << "," << rgba8888(bs.panel.background_color_bottom)
              << "," << rgba8888(bs.panel.border_color)
              << "," << rgba8888(bs.panel.margin_color)
              << "," << bs.panel.margin_left
              << "," << bs.panel.margin_right
              << "," << bs.panel.margin_top
              << "," << bs.panel.margin_bottom
              << "," << bs.panel.border_width_left
              << "," << bs.panel.border_width_right
              << "," << bs.panel.border_width_top
              << "," << bs.panel.border_width_bottom
              << ");" << std::endl
              << "-- " << bs_name << std::endl
              << "INSERT INTO sw_ui_button_styles VALUES (" << (I64)bs_id.value()
              << "," << (I64)font_id.value()
              << "," << rgba8888(bs.text_color)
              << "," << bs.text_scale.x
              << "," << bs.text_scale.y
              << "," << (I64)ps_id.value()
              << ");" << std::endl << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
    glm::vec3 color;
    std::string prefix;

    if (argc >= 5)
    {
        prefix = argv[1];
        color.r = float(atof(argv[2]));
        color.g = float(atof(argv[3]));
        color.b = float(atof(argv[4]));
    }
    else
    {
        std::cerr << "Usage:" << std::endl
                  << "   " << (argc >= 1 ? argv[0] : "btngen") << " <id prefix> <r> <g> <b>" << std::endl;
        return -1;
    }

    ButtonStyle normal;
    normal.text_color = color4(color * 1.3f, 1.0f);
    normal.text_scale = vec2(1.0f, 1.0f);
    normal.panel.background_color_top = color4(color * 0.7f, 0.5f);
    normal.panel.background_color_bottom = color4(color * 0.6f, 0.5f);
    normal.panel.border_color = color4(color * 1.1f, 0.75f);
    normal.panel.border_width_left   = 0.5f;
    normal.panel.border_width_right  = 0.5f;
    normal.panel.border_width_top    = 0.5f;
    normal.panel.border_width_bottom = 0.5f;
    normal.panel.margin_left   = 0.5f;
    normal.panel.margin_right  = 1.5f;
    normal.panel.margin_top    = 0.5f;
    normal.panel.margin_bottom = 1.5f;
    output(normal, prefix + ".normal");

    ButtonStyle hovered(normal);
    hovered.panel.background_color_top.a = 0.7f;
    hovered.panel.background_color_bottom.a = 0.7f;
    hovered.text_color = color4(color * 1.4f, 1.0f);
    hovered.panel.border_color = color4(color * 1.2f, 1.0f);
    output(hovered, prefix + ".hovered");

    ButtonStyle active(hovered);
    active.panel.background_color_top.a = 0.6f;
    active.panel.background_color_bottom.a = 0.6f;
    active.text_color = color4(color * 1.3f, 1.0f);
    active.panel.border_width_left = 0.0f;
    active.panel.border_width_right = 0.0f;
    active.panel.border_width_top = 0.0f;
    active.panel.border_width_bottom = 0.0f;
    active.panel.margin_left = 1.0f;
    active.panel.margin_right = 0.0f;
    active.panel.margin_top = 1.0f;
    active.panel.margin_bottom = 0.0f;
    output(active, prefix + ".active");

    ButtonStyle disabled(normal);
    disabled.panel.background_color_top.a = 0.3f;
    disabled.panel.background_color_bottom.a = 0.3f;
    disabled.text_color *= 0.6f;
    disabled.text_color.a = 1.0f;
    disabled.panel.border_width_left = 0.0f;
    disabled.panel.border_width_right = 0.0f;
    disabled.panel.border_width_top = 0.0f;
    disabled.panel.border_width_bottom = 0.0f;
    disabled.panel.margin_left = 0.0f;
    disabled.panel.margin_right = 1.0f;
    disabled.panel.margin_top = 0.0f;
    disabled.panel.margin_bottom = 1.0f;
    output(disabled, prefix + ".disabled");

    ButtonStyle focused(normal);
    focused.panel.border_color = color4(color * 1.2f, 1.0f);
    focused.panel.border_width_left = 1.0f;
    focused.panel.border_width_right = 1.0f;
    focused.panel.border_width_top = 1.0f;
    focused.panel.border_width_bottom = 1.0f;
    focused.panel.margin_left = 1.0f;
    focused.panel.margin_right = 2.0f;
    focused.panel.margin_top = 1.0f;
    focused.panel.margin_bottom = 2.0f;
    output(focused, prefix + ".focused");

    ButtonStyle focused_hovered(hovered);
    focused_hovered.panel.border_color = color4(color * 1.2f, 1.0f);
    focused_hovered.panel.border_width_left = 1.0f;
    focused_hovered.panel.border_width_right = 1.0f;
    focused_hovered.panel.border_width_top = 1.0f;
    focused_hovered.panel.border_width_bottom = 1.0f;
    focused_hovered.panel.margin_left = 1.0f;
    focused_hovered.panel.margin_right = 2.0f;
    focused_hovered.panel.margin_top = 1.0f;
    focused_hovered.panel.margin_bottom = 2.0f;
    output(focused_hovered, prefix + ".focused_hovered");
    
    ButtonStyle focused_active(active);
    focused_active.panel.border_color = color4(color * 1.2f, 1.0f);
    output(focused_active, prefix + ".focused_active");

    std::cout << ".quit" << std::endl;

    return 0;
}
