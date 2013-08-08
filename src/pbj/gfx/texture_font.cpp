///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/texture_font.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::gfx::TextureFont functions.

#include "pbj/gfx/texture_font.h"

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

} // namespace pbj::gfx
} // namespace pbj
