////////////////////////////////////////////////////////////////////////////////
/// \file   pbj\gfx\shape_triangle.h
///
/// \brief  Declares the triangle class.
#ifndef PBJ_GFX_SHAPE_TRIANGLE_H_
#define PBJ_GFX_SHAPE_TRIANGLE_H_

#include "pbj/_pbj.h"
#include "pbj/gfx/shape.h"
#include "pbj/_math.h"
#include "pbj/gfx/texture.h"

namespace pbj {
namespace gfx {

////////////////////////////////////////////////////////////////////////////
/// \class  ShapeTriangle
///
/// \brief  Josh Douglas / Peter Bartosch
///
/// \author Peter Bartosch
/// \date   2013-08-22
class ShapeTriangle : public Shape
{
public:
    ShapeTriangle() {}
    ~ShapeTriangle() {}

    ////////////////////////////////////////////////////////////////////////
    /// \fn virtual void ShapeTriangle::draw(bool hasTexture)
    ///
    /// \brief  Draws.
    ///
    /// \author Josh Douglas / Peter Bartosch
    /// \date   2013-08-22
    ///
    /// \param  hasTexture  true if this ShapeTriangle has texture.
    ///
    /// \details    Draws a 1 unit per side equilateral triangle around
    ///             this Shape's origin.
    virtual void draw(bool hasTexture)
    {
        if(hasTexture)
        {
            glBegin(GL_TRIANGLES);
                glTexCoord2d(0.0, 1.0); glVertex2f(-0.5f, -0.433f);
                glTexCoord2d(1.0, 1.0); glVertex2f(0.5f, -0.433f);
                glTexCoord2d(0.5, 0.134); glVertex2f(0.0f, 0.433f);
            glEnd();
        }
        else
        {
            glBegin(GL_TRIANGLES);
                glVertex2f(-0.5f, -0.433f);
                glVertex2f(0.5f, -0.433f);
                glVertex2f(0.0f, 0.433f);
            glEnd();
        }
    }
};

} // namespace gfx
} // namespace pbj

#endif
