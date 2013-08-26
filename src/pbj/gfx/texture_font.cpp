///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/texture_font.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::gfx::TextureFont functions.

#include "pbj/gfx/texture_font.h"

#include "pbj/sw/resource_manager.h"
#include <algorithm>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to load a texture font from a sandwich.
/// \param  1 The id of the texture font.
#define PBJ_GFX_TEXTURE_FONT_SQL_LOAD "SELECT texture_id, cap_height " \
            "FROM sw_texture_fonts WHERE id = ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to load character data for a texture font.
/// \param  1 The id of the texture font.
#define PBJ_GFX_TEXTURE_FONT_SQL_LOAD_CHARS "SELECT codepoint, " \
            "tc_x, tc_y, tc_width, tc_height, " \
            "offset_x, offset_y, advance " \
            "FROM sw_texture_font_chars WHERE font_id = ?"

#ifdef BE_ID_NAMES_ENABLED
#define PBJ_GFX_TEXTURE_FONT_SQLID_LOAD         PBJ_GFX_TEXTURE_FONT_SQL_LOAD
#define PBJ_GFX_TEXTURE_FONT_SQLID_LOAD_CHARS   PBJ_GFX_TEXTURE_FONT_SQL_LOAD_CHARS
#else
// TODO: precalculate ids.
#define PBJ_GFX_TEXTURE_FONT_SQLID_LOAD         PBJ_GFX_TEXTURE_FONT_SQL_LOAD
#define PBJ_GFX_TEXTURE_FONT_SQLID_LOAD_CHARS   PBJ_GFX_TEXTURE_FONT_SQL_LOAD_CHARS
#endif

namespace pbj {
namespace gfx {

///////////////////////////////////////////////////////////////////////////////
/// \brief  Constructs a texture font from a texture and a series of
///         TextureFontCharacters describing the location of characters in the
///         texture.
///
/// \param  id Describes the location of the texture font in the database it
///         was loaded from.
/// \param  texture A ConstHandle to the texture for drawing the font.
/// \param  cap_height Not required to actually render text, but describes
///         the height of a typical latin uppercase character.
/// \param  characters A reference to a vector of TextureFontCharacter structs
///         defining the characters in this font.
TextureFont::TextureFont(const Texture& texture, F32 cap_height, const std::vector<TextureFontCharacter>& characters)
    : cap_height_(cap_height),
      texture_(texture)
{
    for(auto i = characters.begin(); i != characters.end(); ++i)
    {
        U32 codepoint = i->codepoint;

        if (codepoint == TextureFontCharacter::cp_invalid)
            default_char_ = *i;
        else if (codepoint < base_chars_size_)
            base_chars_[codepoint] = *i;
        else
        {
            TextureFontCharacter query;
            query.codepoint = codepoint;
            auto j = std::find(ext_chars_.begin(), ext_chars_.end(), query);

            if (j == ext_chars_.end())
            {
                // character hasn't been seen yet
                ext_chars_.push_back(*i);
            }
            else
            {
                // overwrite previous character
                *j = *i;
            }
        }
    }

    std::sort(ext_chars_.begin(), ext_chars_.end());
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Destroys the TextureFont.
TextureFont::~TextureFont()
{
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves the cap height specified for this font.
///
/// \return The cap height.
F32 TextureFont::getCapHeight() const
{
    return cap_height_;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Retrieves a TextureFontCharacter object corresponding to a
///         particular unicode codepoint.
///
/// \details If the codepoint does not have a TextureFontCharacter defined
///         for it, a placeholder character is used.
///
/// \param  codepoint The unicode codepoint to retrieve a TextureFontCharacter
///         for.
/// \return A TextureFontCharacter for drawing the codepoint requested.
const TextureFontCharacter& TextureFont::operator[](U32 codepoint) const
{
    const TextureFontCharacter* ch(nullptr);
    if (codepoint < base_chars_size_)
    {
        ch = &(base_chars_[codepoint]);
    }
    else
    {
        TextureFontCharacter query;
        query.codepoint = codepoint;
        auto i = std::lower_bound(ext_chars_.begin(), ext_chars_.end(), query);

        if (i != ext_chars_.end())
        {
            ch = &(*i);
        }
    }

    if (ch == nullptr || ch->codepoint == TextureFontCharacter::cp_invalid)
        ch = &default_char_;

    return *ch;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Prints out a string using this font, beginning with the baseline
///         at the origin.
///
/// \param  text The string to print.
/// \param  color The base color to modulate by the font texture.
void TextureFont::print(const std::string& text, const color4& color) const
{
    vec2 cursor; // where the current character should be drawn
    F32 scale_x = 1.0f / texture_.getDimensions().x;
    F32 scale_y = 1.0f / texture_.getDimensions().y;

    for (char c : text)
    {
        const TextureFontCharacter& ch = (*this)[c];

        // vertices
        vec2 top_left(cursor + ch.character_offset);
        vec2 bottom_right(top_left.x + ch.texture_dimensions.x, top_left.y - ch.texture_dimensions.y);
        vec2 tex_top_left(ch.texture_offset);
        vec2 tex_bottom_right(tex_top_left + ch.texture_dimensions);
        tex_top_left.x *= scale_x; tex_bottom_right.x *= scale_x;
        tex_top_left.y *= scale_y; tex_bottom_right.y *= scale_y;

        cursor.x += ch.character_advance;

        texture_.enable(GL_MODULATE);
        glColor4fv(glm::value_ptr(color));

        glBegin(GL_QUADS);

        glTexCoord2fv(glm::value_ptr(tex_top_left));        glVertex2fv(glm::value_ptr(top_left));
        glTexCoord2f(tex_top_left.x, tex_bottom_right.y);   glVertex2f(top_left.x, bottom_right.y);
        glTexCoord2fv(glm::value_ptr(tex_bottom_right));     glVertex2fv(glm::value_ptr(bottom_right));
        glTexCoord2f(tex_bottom_right.x, tex_top_left.y);   glVertex2f(bottom_right.x, top_left.y);

        glEnd();
        texture_.disable();
    }
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Calculates the width of a string of text if it were to be
///         output to the screen.
///
/// \param  text The text to measure.
/// \return The width of the text.
F32 TextureFont::calculateTextWidth(const std::string& text) const
{
    vec2 cursor; // where the current character should be drawn
    for (char c : text)
    {
        const TextureFontCharacter& ch = (*this)[c];
        cursor.x += ch.character_advance;
    }
    return cursor.x;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief  Loads a texture font from the specified sandwich database.
///
/// \param  sandwich The database from which to load the TextureFont.
/// \param  texture_id Identifies the TextureFont to load from the database.
/// \param  rm The ResourceManager from which to retrieve the texture for the
///         font.
/// \return A unique_ptr to the font, or an empty unique_ptr if a problem
///         occurs during the load.
std::unique_ptr<TextureFont> loadTextureFont(sw::Sandwich& sandwich, const Id& id, sw::ResourceManager& rm)
{
    std::unique_ptr<TextureFont> result;

    try
    {
        db::StmtCache& cache = sandwich.getStmtCache();
        db::CachedStmt get_font = cache.hold(Id(PBJ_GFX_TEXTURE_FONT_SQLID_LOAD), PBJ_GFX_TEXTURE_FONT_SQL_LOAD);

        get_font.bind(1, id.value());
        if (get_font.step())
        {
            Id texture_id = Id(get_font.getUInt64(0));
            F32 cap_height = float(get_font.getDouble(1));
            std::vector<TextureFontCharacter> chars;

            db::CachedStmt get_chars = cache.hold(Id(PBJ_GFX_TEXTURE_FONT_SQLID_LOAD_CHARS), PBJ_GFX_TEXTURE_FONT_SQL_LOAD_CHARS);

            get_chars.bind(1, id.value());
            while (get_chars.step())
            {
                TextureFontCharacter ch;
                ch.codepoint = get_chars.getInt(0);
                ch.texture_offset.x = float(get_chars.getDouble(1));
                ch.texture_offset.y = float(get_chars.getDouble(2));
                ch.texture_dimensions.x = float(get_chars.getDouble(3));
                ch.texture_dimensions.y = float(get_chars.getDouble(4));
                ch.character_offset.x = float(get_chars.getDouble(5));
                ch.character_offset.y = float(get_chars.getDouble(6));
                ch.character_advance = float(get_chars.getDouble(7));

                chars.push_back(ch);
            }

            const Texture& texture = rm.getTexture(sw::ResourceId(sandwich.getId(), texture_id));


            result.reset(new TextureFont(texture, cap_height, chars));
        }
        else
            throw std::runtime_error("Texture not found!");
    }
    catch (const db::Db::error& err)
    {
        PBJ_LOG(VWarning) << "Database error while loading texture font!" << PBJ_LOG_NL
                          << "   Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "TextureFont ID: " << id << PBJ_LOG_NL
                          << "     Exception: " << err.what() << PBJ_LOG_NL
                          << "           SQL: " << err.sql() << PBJ_LOG_END;
   }
   catch (const std::exception& err)
   {
      PBJ_LOG(VWarning) << "Exception while loading texture font!" << PBJ_LOG_NL
                          << "   Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "TextureFont ID: " << id << PBJ_LOG_NL
                          << "     Exception: " << err.what() << PBJ_LOG_END;
   }

   return result;
}

} // namespace pbj::gfx
} // namespace pbj
