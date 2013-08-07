///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/texture_font.cpp
/// \author Benjamin Crist
///
/// \brief  Implementations of pbj::gfx::TextureFont functions.

#include "pbj/gfx/texture_font.h"

namespace pbj {
namespace gfx {

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
void TextureFont::print(const std::string& text) const
{

}

///////////////////////////////////////////////////////////////////////////////
F32 TextureFont::getTextWidth(const std::string& text) const
{

}

} // namespace pbj::gfx
} // namespace pbj
