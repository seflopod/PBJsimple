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

///////////////////////////////////////////////////////////////////////////////
int create(const std::string& filename)
{
    // first open as a regular sqlite3 database and set up sw_sandwich properties with the sandwich ID.
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

    // After sw_sandwich_properties is set up, we can open the file using pbj::sw::Sandwich().
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

        table_exists.reset();
        table_exists.bind(1, "sw_textures");
        if (!table_exists.step())
        {
            sw->getDb().exec("CREATE TABLE sw_textures\n"
                             "(\n"
                             "   id              INTEGER PRIMARY KEY,\n"
                             "   data            NOT NULL,\n"
                             "   internal_format INTEGER NOT NULL,\n"
                             "   srgb            INTEGER NOT NULL,\n"
                             "   mag_filter      INTEGER NOT NULL,\n"
                             "   min_filter      INTEGER NOT NULL\n"
                             ");");
            PBJ_LOG(pbj::VInfo) << "Created table 'sw_textures'." << PBJ_LOG_END;
        }

        table_exists.reset();
        table_exists.bind(1, "sw_texture_fonts");
        if (!table_exists.step())
        {
            sw->getDb().exec("CREATE TABLE sw_texture_fonts\n"
                             "(\n"
                             "   id         INTEGER PRIMARY KEY,\n"
                             "   texture_id NOT NULL,\n"
                             "   cap_height INTEGER NOT NULL\n"
                             ");");
            PBJ_LOG(pbj::VInfo) << "Created table 'sw_texture_fonts'." << PBJ_LOG_END;
        }

        table_exists.reset();
        table_exists.bind(1, "sw_texture_font_chars");
        if (!table_exists.step())
        {
            sw->getDb().exec("CREATE TABLE sw_texture_font_chars\n"
                             "(\n"
                             "   font_id   INTEGER NOT NULL,\n"
                             "   codepoint INTEGER NOT NULL,\n"
                             "   tc_x      INTEGER NOT NULL,\n"
                             "   tc_y      INTEGER NOT NULL,\n"
                             "   tc_width  INTEGER NOT NULL,\n"
                             "   tc_height INTEGER NOT NULL,\n"
                             "   offset_x  INTEGER NOT NULL,\n"
                             "   offset_y  INTEGER NOT NULL,\n"
                             "   advance   INTEGER NOT NULL,\n"
                             "   PRIMARY KEY (font_id, codepoint)\n"
                             ");");
            PBJ_LOG(pbj::VInfo) << "Created table 'sw_texture_font_chars'." << PBJ_LOG_END;
        }
        
        table_exists.reset();
        table_exists.bind(1, "sw_ui_panel_styles");
        if (!table_exists.step())
        {
            sw->getDb().exec("CREATE TABLE sw_ui_panel_styles\n"
                             "(\n"
                             "   id              INTEGER PRIMARY KEY,\n"
                             "   bg_color_top    INTEGER NOT NULL,\n"
                             "   bg_color_bottom INTEGER NOT NULL,\n"
                             "   border_color    INTEGER NOT NULL,\n"
                             "   margin_color    INTEGER NOT NULL,\n"
                             "   margin_left     REAL NOT NULL,\n"
                             "   margin_right    REAL NOT NULL,\n"
                             "   margin_top      REAL NOT NULL,\n"
                             "   margin_bottom   REAL NOT NULL,\n"
                             "   border_left     REAL NOT NULL,\n"
                             "   border_right    REAL NOT NULL,\n"
                             "   border_top      REAL NOT NULL,\n"
                             "   border_bottom   REAL NOT NULL\n"
                             ");");
            PBJ_LOG(pbj::VInfo) << "Created table 'sw_ui_panel_styles'." << PBJ_LOG_END;
        }

        table_exists.reset();
        table_exists.bind(1, "sw_ui_button_styles");
        if (!table_exists.step())
        {
            sw->getDb().exec("CREATE TABLE sw_ui_button_styles\n"
                             "(\n"
                             "   id             INTEGER PRIMARY KEY,\n"
                             "   font_id        INTEGER NOT NULL,\n"
                             "   text_color     INTEGER NOT NULL,\n"
                             "   text_scale_x   INTEGER NOT NULL,\n"
                             "   text_scale_y   INTEGER NOT NULL,\n"
                             "   panel_style_id INTEGER NOT NULL\n"
                             ");");
            PBJ_LOG(pbj::VInfo) << "Created table 'sw_ui_button_styles'." << PBJ_LOG_END;
        }

        table_exists.reset();
        table_exists.bind(1, "sw_window_settings");
        if (!table_exists.step())
        {
            sw->getDb().exec("CREATE TABLE sw_window_settings\n"
                             "(\n"
                             "   id                INTEGER NOT NULL,\n"
                             "   history_index     INTEGER NOT NULL,\n"
                             "   window_mode       INTEGER NOT NULL,\n"
                             "   system_positioned INTEGER NOT NULL,\n"
                             "   maximized         INTEGER NOT NULL,\n"
                             "   save_pos_on_close INTEGER NOT NULL,\n"
                             "   position_x        INTEGER NOT NULL,\n"
                             "   position_y        INTEGER NOT NULL,\n"
                             "   size_x            INTEGER NOT NULL,\n"
                             "   size_y            INTEGER NOT NULL,\n"
                             "   monitor_index     INTEGER NOT NULL,\n"
                             "   refresh_rate      INTEGER NOT NULL,\n"
                             "   v_sync            INTEGER NOT NULL,\n"
                             "   msaa_level        INTEGER NOT NULL,\n"
                             "   red_bits          INTEGER NOT NULL,\n"
                             "   green_bits        INTEGER NOT NULL,\n"
                             "   blue_bits         INTEGER NOT NULL,\n"
                             "   alpha_bits        INTEGER NOT NULL,\n"
                             "   depth_bits        INTEGER NOT NULL,\n"
                             "   stencil_bits      INTEGER NOT NULL,\n"
                             "   srgb_capable      INTEGER NOT NULL,\n"
                             "   use_custom_gamma  INTEGER NOT NULL,\n"
                             "   custom_gamma      REAL NOT NULL,\n"
                             "   PRIMARY KEY (id, history_index)\n"
                             ");");
            PBJ_LOG(pbj::VInfo) << "Created table 'sw_window_settings'." << PBJ_LOG_END;
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

    return 0;
}
