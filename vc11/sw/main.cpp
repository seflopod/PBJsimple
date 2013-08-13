///////////////////////////////////////////////////////////////////////////////
/// \file   main.cpp
/// \author Benjamin Crist
///
/// \brief  Command-line utility for importing textures to a sandwich

// Auto-link with libraries
#ifdef _WIN32
#pragma comment (lib, "opengl32.lib")
#ifdef DEBUG
#pragma comment (lib, "glew32sd.lib")
#else
#pragma comment (lib, "glew32s.lib")

#endif // DEBUG
#endif // _WIN32

#include <iostream>
#include <string>
#include <algorithm>

#include <stb_image.h>

#include "be/id.h"
#include "pbj/_pbj.h"
#include "pbj/gfx/texture.h"
#include "be/bed/transaction.h"
#include "pbj/sw/sandwich.h"
#include "pbj/sw/sandwich_open.h"
#include "pbj/sw/resource_id.h"

// global variables
std::string cmd_name;
std::string sw_name;
pbj::Id sw_id;
std::string operation;
std::shared_ptr<pbj::sw::Sandwich> sw;


// function prototypes
int create(const std::string& filename);
int texture(const std::string& texture, const std::string& filename, const char** extra, int extra_count);
void displayUsage();


///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
#ifdef DEBUG
   int verbosity = pbj::VAll;
#else
   int verbosity = pbj::VErrorsAndWarnings;
#endif
   
   // Set the appropriate verbosity level
   be::setVerbosity(verbosity);

   // The name of the executable
   cmd_name = (argc > 0 ? argv[0] : "sw");
   sw_name = (argc > 1 ? argv[1] : "<sandwich id>");

   if (argc < 2)
   {
      PBJ_LOG(pbj::VError) << "No sandwich specified!" << PBJ_LOG_END;
      displayUsage();
      return -1;
   }

   sw_id = pbj::Id(sw_name);

   if (argc < 3)
   {
      PBJ_LOG(pbj::VError) << "No operation specified!" << PBJ_LOG_END;
      displayUsage();
      return -1;
   }

   operation = argv[2];
   std::transform(operation.begin(), operation.end(), operation.begin(), tolower);
   if (operation == "tex")
       operation = "texture";


   // if we're creating a sandwich, we don't want to search for an existing one.
   if (operation == "create")
   {
       if (argc < 4)
       {
           PBJ_LOG(pbj::VError) << "No sandwich filename specified!" << PBJ_LOG_END;
           displayUsage();
           return -1;
       }
       return create(argv[3]);
   }

   pbj::sw::readDirectory("./");
   sw = pbj::sw::openWritable(sw_id);
   if (!sw)
   {
       PBJ_LOG(pbj::VError) << "Sandwich could not be opened!" << PBJ_LOG_NL
                            << "Sandwich ID: " << sw_id << PBJ_LOG_END;
       return -1;
   }

   
   if (operation == "texture")
   {
       if (argc < 4)
       {
           PBJ_LOG(pbj::VError) << "No texture name specified!" << PBJ_LOG_END;
           displayUsage();
           return -1;
       }
       else if (argc < 5)
       {
           PBJ_LOG(pbj::VError) << "No texture file specified!" << PBJ_LOG_END;
           displayUsage();
           return -1;
       }
       return texture(std::string(argv[3]), std::string(argv[4]), (const char**)(argv + 5), argc - 5);
   }
   else
   {
      PBJ_LOG(pbj::VError) << "Unrecognized operation!" << PBJ_LOG_END;
      operation = "";
      displayUsage();
      return -1;
   }
}

///////////////////////////////////////////////////////////////////////////////
void displayUsage()
{
    std::cout << std::endl
              << "Usage: " << std::endl;

    if (operation == "" || operation == "create")
        std::cout << "    " << cmd_name << " " << sw_name << " create <filename>" << std::endl;

    if (operation == "" || operation == "texture")
        std::cout << "    " << cmd_name << " " << sw_name << " texture <texture name> <image filename> [srgb] [linear|nearest]" << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
int texture(const std::string& texture, const std::string& filename, const char** extra, int extra_count)
{
    pbj::Id tex_id(texture);
    bool srgb = true;
    pbj::gfx::Texture::FilterMode filter = pbj::gfx::Texture::FM_Nearest;

    try
    {
        int width(0), height(0), comps(0);

        stbi_uc* data = stbi_load(filename.c_str(), &width, &height, &comps, 4);
        if (data == nullptr)
            throw std::runtime_error("Could not load file!");

        pbj::db::Transaction transaction(sw->getDb());
        pbj::db::Stmt update(sw->getDb(), "INSERT OR REPLACE INTO sw_textures (id, data, internal_format, srgb, mag_filter, min_filter) VALUES (?, ?, ?, ?, ?, ?);");
      
        update.bind(1, tex_id.value());
        update.bindBlob(2, data, 4 * width * height);
        update.bind(3, 0);
        update.bind(4, srgb ? 1 : 0);
        update.bind(5, filter);
        update.bind(6, filter);
        update.step();

        transaction.commit();

        stbi_image_free(data);
    }
    catch (const pbj::db::Db::error& e)
    {
        // transaction will be rolled back if not committed,
        // and DB file closed if open.
        PBJ_LOG(pbj::VError) << "SQL error while inserting texture!" << PBJ_LOG_NL
                             << "Sandwich ID: " << sw_id << PBJ_LOG_NL
                             << " Texture ID: " << tex_id << PBJ_LOG_NL
                             << "   Filename: " << filename << PBJ_LOG_NL
                             << "  Exception: " << e.what() << PBJ_LOG_NL
                             << "        SQL: " << e.sql() << PBJ_LOG_END;
        return 1;
    }
    catch (const std::exception& e)
    {
        // transaction will be rolled back if not committed,
        // and DB file closed if open.
        PBJ_LOG(pbj::VError) << "Exception while inserting texture!" << PBJ_LOG_NL
                             << "Sandwich ID: " << sw_id << PBJ_LOG_NL
                             << " Texture ID: " << tex_id << PBJ_LOG_NL
                             << "   Filename: " << filename << PBJ_LOG_NL
                             << "  Exception: " << e.what() << PBJ_LOG_END;
        return 1;
    }

    return 0;
}

int create(const std::string& filename)
{
    try
    {
        pbj::db::Db db(filename);
        pbj::db::Stmt has_prop_table(db, "SELECT name FROM sqlite_master WHERE type = 'table' AND name = 'sw_sandwich_properties' LIMIT 1");

        if (!has_prop_table.step())
        {
            db.exec("CREATE TABLE sw_sandwich_properties\n"
                    "(\n"
                    "   property TEXT PRIMARY KEY, \n"
                    "   value    NUMERIC\n"
                    ");");

            PBJ_LOG(pbj::VInfo) << "Created table 'sw_sandwich_properties'." << PBJ_LOG_END;
        }

        pbj::db::Stmt get_id(db, "SELECT value FROM sw_sandwich_properties WHERE property = 'id' LIMIT 1");
        if (get_id.step())
        {
            pbj::Id db_id(get_id.getUInt64(0));

            if (db_id != sw_id)
            {
                PBJ_LOG(pbj::VError) << "File represents existing sandwich with another ID." << PBJ_LOG_NL
                                     << " Existing Sandwich ID: " << db_id << PBJ_LOG_NL
                                     << "Requested Sandwich ID: " << sw_id << PBJ_LOG_END;
                return 1;
            }
        }
        else
        {
            pbj::db::Stmt put_id(db, "INSERT INTO sw_sandwich_properties (property, value) VALUES ('id', ?);");

            put_id.bind(1, sw_id.value());
            put_id.step();
            PBJ_LOG(pbj::VInfo) << "Sandwich ID set!" << PBJ_LOG_END;
        }
    }
    catch (const pbj::db::Db::error& e)
    {
        // transaction will be rolled back if not committed,
        // and DB file closed if open.
        PBJ_LOG(pbj::VError) << "SQL error while creating sandwich!" << PBJ_LOG_NL
                             << "Sandwich ID: " << sw_id << PBJ_LOG_NL
                             << "   Filename: " << filename << PBJ_LOG_NL
                             << "  Exception: " << e.what() << PBJ_LOG_NL
                             << "        SQL: " << e.sql() << PBJ_LOG_END;
        return 1;
    }
    catch (const std::exception& e)
    {
        // transaction will be rolled back if not committed,
        // and DB file closed if open.
        PBJ_LOG(pbj::VError) << "Exception while creating sandwich!" << PBJ_LOG_NL
                             << "Sandwich ID: " << sw_id << PBJ_LOG_NL
                             << "   Filename: " << filename << PBJ_LOG_NL
                             << "  Exception: " << e.what() << PBJ_LOG_END;
        return 1;
    }

    try
    {
        sw = std::shared_ptr<pbj::sw::Sandwich>(new pbj::sw::Sandwich(filename, false));

        pbj::db::Stmt table_exists(sw->getDb(), "SELECT name FROM sqlite_master WHERE type = 'table' AND name = ? LIMIT 1");



        table_exists.bind(1, "sw_maps");
        if (!table_exists.step())
        {
            sw->getDb().exec("CREATE TABLE sw_maps\n"
                             "(\n"
                             "   id   INTEGER PRIMARY KEY,\n"
                             "   name TEXT NOT NULL\n"
                             ");");
            PBJ_LOG(pbj::VInfo) << "Created table 'sw_maps'." << PBJ_LOG_END;
        }

        table_exists.reset();
        table_exists.bind(1, "sw_map_entities");
        if (!table_exists.step())
        {
            sw->getDb().exec("CREATE TABLE sw_map_entities\n"
                             "(\n"
                             "   map_id      INTEGER NOT NULL,\n"
                             "   id          INTEGER PRIMARY KEY,\n"
                             "   entity_type INTEGER NOT NULL,\n"
                             "   rotation    REAL NOT NULL,\n"
                             "   pos_x       REAL NOT NULL,\n"
                             "   pos_y       REAL NOT NULL,\n"
                             "   scale_x     REAL NOT NULL,\n"
                             "   scale_y     REAL NOT NULL\n"
                             ");");
            PBJ_LOG(pbj::VInfo) << "Created table 'sw_map_entities'." << PBJ_LOG_END;
        }

            /*




CREATE TABLE sw_textures
(
      -- Identifies this texture among all other textures in this sandwich
      id                INTEGER PRIMARY KEY,
      -- The image data for the texture.  Format depends on the value of `data_format`
      data              NOT NULL,
      -- Describes the OpenGL internal format to request for storing the texture on the GPU.
      internal_format   INTEGER NOT NULL,
      -- If non-zero, the texture data will be linearized from sRGB when sampling.
      srgb              INTEGER NOT NULL,
      -- The sampler filtering mode to use for magnification
      mag_filter        INTEGER NOT NULL,
      -- The sampler filtering mode to use for minification
      min_filter        INTEGER NOT NULL
);

CREATE TABLE sw_texture_fonts
(
      id                INTEGER PRIMARY KEY,
      texture_id        INTEGER NOT NULL,
      cap_height        INTEGER NOT NULL
);

CREATE TABLE sw_texture_font_chars
(
      font_id           INTEGER NOT NULL,
      codepoint         INTEGER NOT NULL,
      
      tc_x              INTEGER NOT NULL,
      tc_y              INTEGER NOT NULL,
      
      tc_width          INTEGER NOT NULL,
      tc_height         INTEGER NOT NULL,
      
      offset_x          INTEGER NOT NULL,
      offset_y          INTEGER NOT NULL,
      
      advance           INTEGER NOT NULL,
      
      PRIMARY KEY (font_id, codepoint)
);

CREATE TABLE sw_ui_panel_styles
(
      id                INTEGER PRIMARY KEY,
      bg_color_top      INTEGER NOT NULL,
      bg_color_bottom   INTEGER NOT NULL,
      border_color      INTEGER NOT NULL,
      margin_color      INTEGER NOT NULL,
      margin_left       REAL NOT NULL,
      margin_right      REAL NOT NULL,
      margin_top        REAL NOT NULL,
      margin_bottom     REAL NOT NULL,
      border_left       REAL NOT NULL,
      border_right      REAL NOT NULL,
      border_top        REAL NOT NULL,
      border_bottom     REAL NOT NULL
);

CREATE TABLE sw_ui_button_styles
(
      id                INTEGER PRIMARY KEY,
      font_id           INTEGER NOT NULL,
      text_color        INTEGER NOT NULL,
      text_scale_x      REAL NOT NULL,
      text_scale_y      REAL NOT NULL,
      panel_style_id    INTEGER NOT NULL
);

CREATE TABLE sw_window_settings
(
      -- Identifies this set of window settings, allowing multiple windows to
      -- store their settings independently.
      id                INTEGER NOT NULL,
      -- When window settings are changed (except when saving new window
      -- position/size on window close), a new history index is created so
      -- that the old settings can be restored if the new ones are bad.
      history_index     INTEGER NOT NULL,
      
      -- Determines whether the window is fullscreen or not, and how much
      -- control the user has over its position/size.
      --    0  Windowed, resizable
      --    1  Windowed, not resizable
      --    2  Windowed, undecorated, resizable
      --    3  Windowed, undecorated, not resizable
      --    7  Windowed Fullscreen
      --    8  Exclusive Fullscreen
      window_mode       INTEGER NOT NULL,
      -- If 0, the window should be moved to `position_x`,`position_y` after it
      -- is created.  Ignored in fullscreen exclusive mode.
      system_positioned INTEGER NOT NULL,
      maximized         INTEGER NOT NULL,
      save_pos_on_close INTEGER NOT NULL,
      position_x        INTEGER NOT NULL,
      position_y        INTEGER NOT NULL,
      size_x            INTEGER NOT NULL,
      size_y            INTEGER NOT NULL,
      monitor_index     INTEGER NOT NULL,
      
      refresh_rate      INTEGER NOT NULL,
      v_sync            INTEGER NOT NULL,
      msaa_level        INTEGER NOT NULL,
      
      red_bits          INTEGER NOT NULL,
      green_bits        INTEGER NOT NULL,
      blue_bits         INTEGER NOT NULL,
      alpha_bits        INTEGER NOT NULL,
      depth_bits        INTEGER NOT NULL,
      stencil_bits      INTEGER NOT NULL,
      
      srgb_capable      INTEGER NOT NULL,
      use_custom_gamma  INTEGER NOT NULL,
      custom_gamma      REAL NOT NULL,
      
      PRIMARY KEY (id, history_index)
);
*/

    }
    catch (const pbj::db::Db::error& e)
    {
        // transaction will be rolled back if not committed,
        // and DB file closed if open.
        PBJ_LOG(pbj::VError) << "SQL error while creating sandwich!" << PBJ_LOG_NL
                             << "Sandwich ID: " << sw_id << PBJ_LOG_NL
                             << "   Filename: " << filename << PBJ_LOG_NL
                             << "  Exception: " << e.what() << PBJ_LOG_NL
                             << "        SQL: " << e.sql() << PBJ_LOG_END;
        return 1;
    }
    catch (const std::exception& e)
    {
        // transaction will be rolled back if not committed,
        // and DB file closed if open.
        PBJ_LOG(pbj::VError) << "Exception while creating sandwich!" << PBJ_LOG_NL
                             << "Sandwich ID: " << sw_id << PBJ_LOG_NL
                             << "   Filename: " << filename << PBJ_LOG_NL
                             << "  Exception: " << e.what() << PBJ_LOG_END;
        return 1;
    }

    return 0;
}
