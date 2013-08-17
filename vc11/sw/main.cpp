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
#include <fstream>

#include "pugixml.hpp"
#include "stb_image.h"

#include "pbj/win_unicode.h"

#include "be/id.h"
#include "pbj/_pbj.h"

#include "pbj/gfx/texture.h"
#include "be/bed/transaction.h"
#include "pbj/sw/sandwich.h"
#include "pbj/gfx/texture_font_character.h"
#include "pbj/sw/sandwich_open.h"
#include "pbj/sw/resource_id.h"

// global variables
std::string cmd_name;
std::string sw_name;
pbj::Id sw_id;
std::string operation;
std::shared_ptr<pbj::sw::Sandwich> sw;

// Enums
enum ListType
{
    LTProperties = 0x1,
    LTTextures = 0x2,
    LTFonts = 0x4,
    LTAll = 0xFFFFFFFF
};

// function prototypes
int create(const std::string& filename);
int list(ListType type);
int prop(const std::string& prop, const std::string& value);
int texture(const std::string& texture, const std::string& filename, const char** extra, int extra_count);
int textureFont(const std::string& font, const std::string& filename);
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
    else if (operation == "prop")
        operation = "property";
    else if (operation == "texturefont" || operation == "texfont")
        operation = "font";


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

    if (operation == "list")
    {
        ListType type = LTAll;
        if (argc >= 4)
        {
            std::string tstr(argv[3]);
            std::transform(tstr.begin(), tstr.end(), tstr.begin(), tolower);
            if (tstr == "properties" || tstr == "props")
                type = LTProperties;
            else if (tstr == "textures")
                type = LTTextures;
            else if (tstr == "fonts" || tstr == "texfonts" || tstr == "texturefonts")
                type = LTFonts;
        }
        return list(type);
    }
    else if (operation == "property")
    {
        if (argc < 4)
        {
            PBJ_LOG(pbj::VError) << "No property name specified!" << PBJ_LOG_END;
            displayUsage();
            return -1;
        }
        else if (argc < 5)
        {
            PBJ_LOG(pbj::VError) << "No value specified!" << PBJ_LOG_END;
            displayUsage();
            return -1;
        }
        return prop(argv[3], argv[4]);
    }
    else if (operation == "texture")
    {
        if (argc < 4)
        {
            PBJ_LOG(pbj::VError) << "No texture id specified!" << PBJ_LOG_END;
            displayUsage();
            return -1;
        }
        else if (argc < 5)
        {
            PBJ_LOG(pbj::VError) << "No texture file specified!" << PBJ_LOG_END;
            displayUsage();
            return -1;
        }
        return texture(argv[3], argv[4], (const char**)(argv + 5), argc - 5);
    }
    else if (operation == "font")
    {
        if (argc < 4)
        {
            PBJ_LOG(pbj::VError) << "No font id specified!" << PBJ_LOG_END;
            displayUsage();
            return -1;
        }
        else if (argc < 5)
        {
            PBJ_LOG(pbj::VError) << "No Angelcode BMFont XML spec specified!" << PBJ_LOG_END;
            displayUsage();
            return -1;
        }

        return textureFont(argv[3], argv[4]);
    }
    else if (operation == "vacuum")
    {
        sw->getDb().vacuum();
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

    if (operation == "" || operation == "list")
        std::cout << "    " << cmd_name << " " << sw_name << " list [all|properties|textures]" << std::endl;

    if (operation == "" || operation == "property")
        std::cout << "    " << cmd_name << " " << sw_name << " property <property name> <value>" << std::endl;

    if (operation == "" || operation == "texture")
        std::cout << "    " << cmd_name << " " << sw_name << " texture <texture id> <image filename> [srgb] [linear|nearest]" << std::endl;

    if (operation == "" || operation == "font")
        std::cout << "    " << cmd_name << " " << sw_name << " font <font id> <bmfont xml filename>" << std::endl;

    if (operation == "" || operation == "vacuum")
        std::cout << "    " << cmd_name << " " << sw_name << " vacuum" << std::endl;
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

///////////////////////////////////////////////////////////////////////////////
int list(ListType type)
{
    try
    {
        if (type & LTProperties)
        {
            std::cout << "Properties:" << std::endl;

            pbj::db::Stmt get_props(sw->getDb(), "SELECT property, value FROM sw_sandwich_properties ORDER BY property ASC");
            while (get_props.step())
            {
                std::string prop = get_props.getText(0);
                std::string value = get_props.getText(1);

                std::cout << "   " << prop << ":";

                for (int i = prop.length(); i < 20; ++i)
                    std::cout << ' ';

                if (prop == "id")
                    std::cout << pbj::Id(get_props.getUInt64(1)) << std::endl;
                else
                    std::cout << value << std::endl;
            }

            std::cout << std::endl;
        }

        if (type & LTTextures)
        {
            std:: cout << "Textures:" << std::endl;

            pbj::db::Stmt get(sw->getDb(), "SELECT id, data FROM sw_textures");
            while (get.step())
            {
                std::cout << "   " << pbj::Id(get.getUInt64(0)) << std::endl;
            }

            std::cout << std::endl;
        }

        if (type & LTFonts)
        {
            std:: cout << "TextureFonts:" << std::endl;

            pbj::db::Stmt get(sw->getDb(), "SELECT id FROM sw_texture_fonts");
            while (get.step())
            {
                std::cout << "   " << pbj::Id(get.getUInt64(0)) << std::endl;
            }

            std::cout << std::endl;
        }
    }
    catch (const pbj::db::Db::error& e)
    {
        // transaction will be rolled back if not committed,
        // and DB file closed if open.
        PBJ_LOG(pbj::VError) << "SQL error while listing resources!" << PBJ_LOG_NL
                             << "Sandwich ID: " << sw_id << PBJ_LOG_NL
                             << "  Exception: " << e.what() << PBJ_LOG_NL
                             << "        SQL: " << e.sql() << PBJ_LOG_END;
        return 1;
    }
    catch (const std::exception& e)
    {
        // transaction will be rolled back if not committed,
        // and DB file closed if open.
        PBJ_LOG(pbj::VError) << "Exception while listing resources!" << PBJ_LOG_NL
                             << "Sandwich ID: " << sw_id << PBJ_LOG_NL
                             << "  Exception: " << e.what() << PBJ_LOG_END;
        return 1;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
int prop(const std::string& prop, const std::string& value)
{
    try
    {
        pbj::db::Stmt set_prop(sw->getDb(), "INSERT OR REPLACE INTO sw_sandwich_properties (property, value) VALUES (?, ?);");
        set_prop.bind(1, prop);
        set_prop.bind(2, value);
        set_prop.step();
    }
    catch (const pbj::db::Db::error& e)
    {
        // transaction will be rolled back if not committed,
        // and DB file closed if open.
        PBJ_LOG(pbj::VError) << "SQL error while setting property!" << PBJ_LOG_NL
                             << "Sandwich ID: " << sw_id << PBJ_LOG_NL
                             << "   Property: " << prop << PBJ_LOG_NL
                             << "  New Value: " << value << PBJ_LOG_NL
                             << "  Exception: " << e.what() << PBJ_LOG_NL
                             << "        SQL: " << e.sql() << PBJ_LOG_END;
        return 1;
    }
    catch (const std::exception& e)
    {
        // transaction will be rolled back if not committed,
        // and DB file closed if open.
        PBJ_LOG(pbj::VError) << "Exception while setting property!" << PBJ_LOG_NL
                             << "Sandwich ID: " << sw_id << PBJ_LOG_NL
                             << "   Property: " << prop << PBJ_LOG_NL
                             << "  New Value: " << value << PBJ_LOG_NL
                             << "  Exception: " << e.what() << PBJ_LOG_END;
        return 1;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
int texture(const std::string& texture, const std::string& filename, const char** extra, int extra_count)
{
    pbj::Id tex_id(texture);
    bool srgb = true;
    pbj::gfx::Texture::FilterMode filter = pbj::gfx::Texture::FM_Nearest;

    for (int i = 0; i < extra_count; ++i)
    {
        std::string cmd = extra[i];
        std::transform(cmd.begin(), cmd.end(), cmd.begin(), tolower);
        if (cmd == "srgb")
            srgb = true;
        else if (cmd == "no-srgb")
            srgb = false;
        else if (cmd == "linear")
            filter = pbj::gfx::Texture::FM_Linear;
        else if (cmd == "nearest")
            filter = pbj::gfx::Texture::FM_Nearest;
    }

    try
    {
        std::vector<pbj::U8> image_data;
        std::ifstream ifs(filename, std::ifstream::binary);

        while (ifs)
        {
            int ch = ifs.get();
            if (ch != std::char_traits<char>::eof())
                image_data.push_back(ch);
        }


        //stbi_uc* data = stbi_load(filename.c_str(), &width, &height, &comps, 4);
        //if (data == nullptr)
            //throw std::runtime_error("Could not load file!");

        pbj::db::Transaction transaction(sw->getDb());
        pbj::db::Stmt update(sw->getDb(), "INSERT OR REPLACE INTO sw_textures (id, data, internal_format, srgb, mag_filter, min_filter) VALUES (?, ?, ?, ?, ?, ?);");
      
        update.bind(1, tex_id.value());
        update.bindBlob(2, image_data.data(), image_data.size());
        update.bind(3, 0);
        update.bind(4, srgb ? 1 : 0);
        update.bind(5, filter);
        update.bind(6, filter);
        update.step();

        transaction.commit();

        //stbi_image_free(data);

        PBJ_LOG(pbj::VInfo) << "Inserted Texture! " << PBJ_LOG_NL
                            << "Texture ID: " << tex_id << PBJ_LOG_END;
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
int textureFont(const std::string& font, const std::string& filename)
{
    pbj::Id font_id(font);
    pbj::Id texture_id(font + ".texture");

    try
    {
        std::wstring wide_filename(pbj::widen(filename)); 

        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(wide_filename.c_str());

        if (result.status != pugi::xml_parse_status::status_ok)
        {
            std::string status;
            switch (result.status)
            {
                case pugi::xml_parse_status::status_bad_attribute:          status = "bad_attribute"; break;
                case pugi::xml_parse_status::status_bad_cdata:              status = "bad_cdata"; break;
                case pugi::xml_parse_status::status_bad_comment:            status = "bad_comment"; break;
                case pugi::xml_parse_status::status_bad_doctype:            status = "bad_doctype"; break;
                case pugi::xml_parse_status::status_bad_end_element:        status = "bad_end_element"; break;
                case pugi::xml_parse_status::status_bad_pcdata:             status = "bad_pcdata"; break;
                case pugi::xml_parse_status::status_bad_pi:                 status = "bad_pi"; break;
                case pugi::xml_parse_status::status_bad_start_element:      status = "bad_start_element"; break;
                case pugi::xml_parse_status::status_end_element_mismatch:   status = "end_element_mismatch"; break;
                case pugi::xml_parse_status::status_file_not_found:         status = "file_not_found"; break;
                case pugi::xml_parse_status::status_internal_error:         status = "internal_error"; break;
                case pugi::xml_parse_status::status_io_error:               status = "io_error"; break;
                case pugi::xml_parse_status::status_out_of_memory:          status = "out_of_memory"; break;
                case pugi::xml_parse_status::status_unrecognized_tag:       status = "unrecognized_tag"; break;
                default:                                                    status = std::to_string(result.status); break;
            }

            PBJ_LOG(pbj::VWarning) << "Failed to load XML BMFont!" << PBJ_LOG_NL
                                   << "     Pugi Status: " << status << PBJ_LOG_NL
                                   << "Pugi Description: " << result.description() << PBJ_LOG_END;

            throw std::runtime_error("Could not load BMFont XML spec.");
        }

        pugi::xml_node el_font = doc.document_element();
        pugi::xml_node el_common = el_font.child("common");
        pugi::xml_node el_chars = el_font.child("chars");

        pugi::xml_attribute at_lineHeight = el_common.attribute("lineHeight");
        pugi::xml_attribute at_base = el_common.attribute("base");
        pugi::xml_attribute at_scale_w = el_common.attribute("scaleW");
        pugi::xml_attribute at_scale_h = el_common.attribute("scaleH");

        pbj::ivec2 scale(at_scale_w.as_int(), at_scale_h.as_int());
        pbj::I32 y_offset = at_base.as_int();

        if (scale.x <= 0 || scale.y <= 0)
        {
            PBJ_LOG(pbj::VWarning) << "Failed to load XML BMFont: invalid scaling!" << PBJ_LOG_NL
                                   << "Common scaleW: " << scale.x << PBJ_LOG_NL
                                   << "Common scaleH: " << scale.y << PBJ_LOG_END;

            throw std::runtime_error("Could not load BMFont XML spec: invalid scaling.");
        }

        std::vector<pbj::gfx::TextureFontCharacter> chars;

        pbj::F32 cap_height(0);
        bool x_cap_found(false);

        for (auto i(el_chars.begin()), end(el_chars.end()); i != end; ++i)
        {
            if (std::string(i->name()) == "char")
            {
                pugi::xml_node& el_char = *i;
                pugi::xml_attribute at_page = el_char.attribute("page");
                if (at_page.as_int() == 0)
                {
                    pugi::xml_attribute at_id = el_char.attribute("id");
                    pugi::xml_attribute at_x = el_char.attribute("x");
                    pugi::xml_attribute at_y = el_char.attribute("y");
                    pugi::xml_attribute at_width = el_char.attribute("width");
                    pugi::xml_attribute at_height = el_char.attribute("height");
                    pugi::xml_attribute at_xoffset = el_char.attribute("xoffset");
                    pugi::xml_attribute at_yoffset = el_char.attribute("yoffset");
                    pugi::xml_attribute at_xadvance = el_char.attribute("xadvance");

                    pbj::gfx::TextureFontCharacter ch;

                    ch.character_advance = at_xadvance.as_float();
                    ch.character_offset = pbj::vec2(at_xoffset.as_float(), y_offset - at_yoffset.as_float());
                    ch.codepoint = at_id.as_uint();
                    ch.texture_dimensions = pbj::vec2(at_width.as_float(), at_height.as_float());
                    ch.texture_offset = pbj::vec2(at_x.as_float(), at_y.as_float());

                    if (ch.codepoint < 0)
                        ch.codepoint = pbj::gfx::TextureFontCharacter::cp_invalid;

                    chars.push_back(ch);

                    if (!x_cap_found && ch.codepoint >= 'A' && ch.codepoint <= 'Z')
                    {
                        if (ch.codepoint == 'X')
                            x_cap_found = true;

                        cap_height = ch.character_offset.y;
                    }
                }
            }
        }


        pbj::db::Transaction transaction(sw->getDb());

        pbj::db::Stmt remove_chars(sw->getDb(), "DELETE FROM sw_texture_font_chars WHERE font_id = ?");
        remove_chars.bind(1, font_id.value());
        remove_chars.step();

        pbj::db::Stmt insert(sw->getDb(), "INSERT OR REPLACE INTO sw_texture_fonts (id, texture_id, cap_height) VALUES (?, ?, ?);");
        insert.bind(1, font_id.value());
        insert.bind(2, texture_id.value());
        insert.bind(3, cap_height);
        insert.step();

        pbj::db::Stmt insert_char(sw->getDb(), "INSERT INTO sw_texture_font_chars (font_id, codepoint, tc_x, tc_y, tc_width, tc_height, offset_x, offset_y, advance) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?);");
        insert_char.bind(1, font_id.value());
        for (auto i(chars.begin()), end(chars.end()); i != end; ++i)
        {
            const pbj::gfx::TextureFontCharacter& ch = *i;
            insert_char.bind(2, ch.codepoint);
            insert_char.bind(3, ch.texture_offset.x);
            insert_char.bind(4, ch.texture_offset.y);
            insert_char.bind(5, ch.texture_dimensions.x);
            insert_char.bind(6, ch.texture_dimensions.y);
            insert_char.bind(7, ch.character_offset.x);
            insert_char.bind(8, ch.character_offset.y);
            insert_char.bind(9, ch.character_advance);
            insert_char.step();
            insert_char.reset();
        }

        transaction.commit();

        PBJ_LOG(pbj::VInfo) << "TextureFont Texture ID: " << texture_id << PBJ_LOG_END;
        PBJ_LOG(pbj::VInfo) << "Inserted " << chars.size() << " TextureFont Characters." << PBJ_LOG_END;
    }
    catch (const pbj::db::Db::error& e)
    {
        // transaction will be rolled back if not committed,
        // and DB file closed if open.
        PBJ_LOG(pbj::VError) << "SQL error while adding texture font!" << PBJ_LOG_NL
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
        PBJ_LOG(pbj::VError) << "Exception while adding texture font!" << PBJ_LOG_NL
                             << "Sandwich ID: " << sw_id << PBJ_LOG_NL
                             << "   Filename: " << filename << PBJ_LOG_NL
                             << "  Exception: " << e.what() << PBJ_LOG_END;
        return 1;
    }

    return 0;
}