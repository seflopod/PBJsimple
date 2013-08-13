#ifndef MATERIAL_H
#include "pbj\gfx\material.h"
#endif

namespace pbj{
namespace gfx{

Material::Material()
{
	_color = color4(0,1,0,1);
	_texId = 0;
}

Material::~Material()
{}

GLuint Material::getTextureId() const
{
	return _texId;
}

void Material::setTextureId(GLuint texID)
{
	_texId = texID;
}


} // namespace gfx
} // namespace pbj