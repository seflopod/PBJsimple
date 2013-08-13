////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\src\pbj\gfx\material.cpp
///
/// \brief	Implements the Material class.
////////////////////////////////////////////////////////////////////////////////
#ifndef MATERIAL_H
#include "pbj\gfx\material.h"
#endif

namespace pbj{
namespace gfx{

////////////////////////////////////////////////////////////////////////////////
/// \brief	Default Material constructor
///
/// \author	Josh Douglas
/// \date	2013-08-13
////////////////////////////////////////////////////////////////////////////////
Material::Material()
{
	_color = color4(0,1,0,1);
	_texId = 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	Default Material destructor
///
/// \author	Josh Douglas
/// \date	2013-08-13
////////////////////////////////////////////////////////////////////////////////
Material::~Material()
{}

////////////////////////////////////////////////////////////////////////////////
/// \brief	Get function to return a texID to be used to attach a texture to an
///			object
///
/// \author	Josh Douglas
/// \date	2013-08-13
////////////////////////////////////////////////////////////////////////////////
GLuint Material::getTextureId() const
{
	return _texId;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	Set function to set _texId equal to texID
///
/// \author	Josh Douglas
/// \date	2013-08-13
////////////////////////////////////////////////////////////////////////////////
void Material::setTextureId(GLuint texID)
{
	_texId = texID;
}

} // namespace gfx
} // namespace pbj