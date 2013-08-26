////////////////////////////////////////////////////////////////////////////////
/// \file   pbj\gfx\material.h
///
/// \brief  Declares the Material class.

#ifndef PBJ_GFX_MATERIAL_H_
#define PBJ_GFX_MATERIAL_H_

#include "pbj/_pbj.h"
#include "pbj/_math.h"
#include "pbj/gfx/texture.h"
#include "pbj/sw/resource_manager.h"

namespace pbj {
namespace gfx {

////////////////////////////////////////////////////////////////////////////////
/// \brief  Defines a color and optional texture and texture mode parameters
///         which define the appearance of a shape.
///
/// \author Peter Bartosh
/// \author Josh Douglas
/// \author Ben Crist
class Material
{
public:
    Material(const sw::ResourceId& id, const color4& color, const Texture* texture, GLenum texture_mode);
    ~Material();

    const color4& getColor() const;
    const Texture* getTexture() const;
    GLenum getTextureMode() const;

    void use() const;

    const sw::ResourceId& getId() const;

private:
    sw::ResourceId id_;
    color4 color_;
    const Texture* tex_;
    GLenum tex_mode_;

    Material(const Material&);
    void operator=(const Material&);
};

std::unique_ptr<Material> loadMaterial(sw::Sandwich& sandwich, const Id& id, sw::ResourceManager& rm);

} //namespace gfx
} //namespace pbj

#endif
