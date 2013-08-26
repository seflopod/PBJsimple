////////////////////////////////////////////////////////////////////////////////
/// \file   gfx\shape_square.h
///
/// \brief  Declares the square class.
#ifndef PBJ_GFX_SHAPE_SQUARE_H_
#define PBJ_GFX_SHAPE_SQUARE_H_

#include "pbj/_pbj.h"
#include "pbj/gfx/shape.h"
#include "pbj/_math.h"
#include "pbj/_gl.h"

namespace pbj {
namespace gfx {
////////////////////////////////////////////////////////////////////////////
/// \class  ShapeSquare
///
/// \brief  Shape square.
///
/// \author Peter Bartosch
/// \date   2013-08-22
class ShapeSquare : public Shape
{
public:
    ShapeSquare() {}
    ~ShapeSquare() {}

    ////////////////////////////////////////////////////////////////////////
    /// \fn virtual void ShapeTriangle::draw(bool hasTexture)
    ///
    /// \brief  Draws.
    ///
    /// \author Peter Bartosch
    /// \date   2013-08-22
    ///
    /// \param  hasTexture  true if this ShapeTriangle has texture.
    ///
    /// \details    Draws a unit square around this Shape's origin.
    virtual void draw(bool hasTexture)
    {
        if(hasTexture)
        {
            glBegin(GL_QUADS);
                glTexCoord2d(0.0, 0.0); glVertex2f(-0.5f, 0.5f);
                glTexCoord2d(1.0, 0.0); glVertex2f(0.5f, 0.5f);
                glTexCoord2d(1.0, 1.0); glVertex2f(0.5f, -0.5f);
                glTexCoord2d(0.0, 1.0); glVertex2f(-0.5f, -0.5f);
            glEnd();
        }
        else
        {
            glBegin(GL_QUADS);
             glVertex2f(-0.5f, 0.5f);
             glVertex2f(0.5f, 0.5f);;
             glVertex2f(0.5f, -0.5f);
             glVertex2f(-0.5f, -0.5f);
            glEnd();
        }
    }
};

} //namespace gfx
} //namspace pbj

#endif
