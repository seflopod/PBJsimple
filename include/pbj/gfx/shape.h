////////////////////////////////////////////////////////////////////////////////
/// \file   Z:\Documents\PBJsimple\include\pbj\gfx\shape.h
///
/// \brief  Declares the shape class.
////////////////////////////////////////////////////////////////////////////////
#ifndef PBJ_GFX_SHAPE_H_
#define PBJ_GFX_SHAPE_H_

#include "pbj/_pbj.h"

namespace pbj {
namespace gfx {

////////////////////////////////////////////////////////////////////////////
/// \class  Shape
///
/// \brief  An interface for shapes that can be drawn.  In 3D this would be
///         Mesh.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \details    This interface is to make sure all drawable shapes actually
///             have a draw method.
////////////////////////////////////////////////////////////////////////////
class Shape
{
public:
    virtual void draw(bool hasTexture) = 0;
};

} //namespace gfx
} //namspace pbj

#endif
