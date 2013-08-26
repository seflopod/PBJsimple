///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/scene/ui_styles.cpp
/// \author Benjamin Crist
///
/// \brief  Loader functions for UI element style structures.

#include "pbj/scene/ui_styles.h"

#include "pbj/sw/resource_manager.h"
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to load a UIPanelStyle from a sandwich
#define PBJ_SCENE_UI_STYLES_SQL_LOAD_PSTYLE "SELECT bg_color_top, bg_color_bottom, " \
            "border_color, margin_color, " \
            "margin_left, margin_right, margin_top, margin_bottom, " \
            "border_left, border_right, border_top, border_bottom " \
            "FROM sw_ui_panel_styles WHERE id = ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to load a UIButtonStyle from a sandwich
#define PBJ_SCENE_UI_STYLES_SQL_LOAD_BSTYLE "SELECT font_id, text_color, " \
            "text_scale_x, text_scale_y, panel_style_id " \
            "FROM sw_ui_button_styles WHERE id = ?"

#ifdef BE_ID_NAMES_ENABLED
#define PBJ_SCENE_UI_STYLES_SQLID_LOAD_PSTYLE PBJ_SCENE_UI_STYLES_SQL_LOAD_PSTYLE
#define PBJ_SCENE_UI_STYLES_SQLID_LOAD_BSTYLE PBJ_SCENE_UI_STYLES_SQL_LOAD_BSTYLE
#else
// TODO: precalculate ids.
#define PBJ_SCENE_UI_STYLES_SQLID_LOAD_PSTYLE PBJ_SCENE_UI_STYLES_SQL_LOAD_PSTYLE
#define PBJ_SCENE_UI_STYLES_SQLID_LOAD_BSTYLE PBJ_SCENE_UI_STYLES_SQL_LOAD_BSTYLE
#endif

namespace pbj {
namespace scene {

///////////////////////////////////////////////////////////////////////////////
/// \brief  loads a UIPanelStyle structure from a sandwich.
///
/// \param  sandwich The database from which to load the resource.
/// \param  id Identifies the panel style object to load from the database.
/// \return The UIPanelStyle loaded from the database.  If there are any
///         problems with the load, the returned UIPanelStyle may be invalid.
UIPanelStyle loadUIPanelStyle(sw::Sandwich& sandwich, const Id& id)
{
    UIPanelStyle result;

    try
    {
        db::StmtCache& cache = sandwich.getStmtCache();
        db::CachedStmt get = cache.hold(Id(PBJ_SCENE_UI_STYLES_SQLID_LOAD_PSTYLE), PBJ_SCENE_UI_STYLES_SQL_LOAD_PSTYLE);

        get.bind(1, id.value());
        if (get.step())
        {
            result.background_color_top = get.getColor(0);
            result.background_color_bottom = get.getColor(1);
            result.border_color = get.getColor(2);
            result.margin_color = get.getColor(3);
            result.margin_left = float(get.getDouble(4));
            result.margin_right = float(get.getDouble(5));
            result.margin_top = float(get.getDouble(6));
            result.margin_bottom = float(get.getDouble(7));
            result.border_width_left = float(get.getDouble(8));
            result.border_width_right = float(get.getDouble(9));
            result.border_width_top = float(get.getDouble(10));
            result.border_width_bottom = float(get.getDouble(11));
        }
        else
            throw std::runtime_error("Panel Style not found!");
    }
    catch (const db::Db::error& err)
    {
        PBJ_LOG(VWarning) << "Database error while loading panel style!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "Resource ID: " << id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_NL
                          << "        SQL: " << err.sql() << PBJ_LOG_END;
    }
    catch (const std::exception& err)
    {
        PBJ_LOG(VWarning) << "Exception while loading panel style!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "Resource ID: " << id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_END;
    }

    return result;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  loads a UIButtonStyle structure from a sandwich.
///
/// \param  sandwich The database from which to load the resource.
/// \param  id Identifies the button style object to load from the database.
/// \return The UIButtonStyle loaded from the database.  If there are any
///         problems with the load, the returned UIButtonStyle may be invalid.
UIButtonStyle loadUIButtonStyle(sw::Sandwich& sandwich, const Id& id, sw::ResourceManager& rm)
{
    UIButtonStyle result;

    try
    {
        db::StmtCache& cache = sandwich.getStmtCache();
        db::CachedStmt get = cache.hold(Id(PBJ_SCENE_UI_STYLES_SQLID_LOAD_BSTYLE), PBJ_SCENE_UI_STYLES_SQL_LOAD_BSTYLE);

        get.bind(1, id.value());
        if (get.step())
        {
            result.font = &rm.getTextureFont(sw::ResourceId(sandwich.getId(), Id(get.getUInt64(0))));
            result.text_color = get.getColor(1);
            result.text_scale.x = float(get.getDouble(2));
            result.text_scale.y = float(get.getDouble(3));
            result.panel = rm.getUIPanelStyle(sw::ResourceId(sandwich.getId(), Id(get.getUInt64(4))));
        }
        else
            throw std::runtime_error("Button Style not found!");
    }
    catch (const db::Db::error& err)
    {
        PBJ_LOG(VWarning) << "Database error while loading button style!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "Resource ID: " << id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_NL
                          << "        SQL: " << err.sql() << PBJ_LOG_END;
    }
    catch (const std::exception& err)
    {
        PBJ_LOG(VWarning) << "Exception while loading button style!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "Resource ID: " << id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_END;
    }

    return result;
}

} // namespace pbj::scene
} // namespace pbj
