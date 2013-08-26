///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/gfx/texture_font.h
/// \author Benjamin Crist
///
/// \brief  pbj::gfx::TextureFont class header.

#ifndef PBJ_GFX_TEXTURE_FONT_H_
#define PBJ_GFX_TEXTURE_FONT_H_

#include "pbj/gfx/texture.h"
#include "pbj/gfx/texture_font_character.h"

namespace pbj {
namespace sw {

class ResourceManager;

}

namespace gfx {

class TextureFontText;

///////////////////////////////////////////////////////////////////////////////
/// \brief  Allows the drawing of text in OpenGL through the use of a font
///         sprite sheet.
///
/// \author Ben Crist
///
/// \details Fonts are specified in a manner closely resembling the Angelcode
///         BMFont format; the texture coordinates and dimensions are set
///         independently of the offset from the cursor location on screen
///         and independently of the cursor advance for the character.
class TextureFont
{
public:
    TextureFont(const Texture& texture, F32 cap_height, const std::vector<TextureFontCharacter>& characters);
    ~TextureFont();

    F32 getCapHeight() const;

    const TextureFontCharacter& operator[](U32 codepoint) const;

    void print(const std::string& text, const color4& color) const;
    F32 calculateTextWidth(const std::string& text) const;

private:
    const Texture& texture_;

    F32 cap_height_;

    static const size_t base_chars_size_ = 128;
    TextureFontCharacter default_char_;
    TextureFontCharacter base_chars_[base_chars_size_];
    std::vector<TextureFontCharacter> ext_chars_;
};


std::unique_ptr<TextureFont> loadTextureFont(sw::Sandwich& sandwich, const Id& id, sw::ResourceManager& rm);

} // namespace pbj::gfx
} // namespace pbj

#endif
