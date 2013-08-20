////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\include\pbj\gfx\shape_triangle.h
///
/// \brief	Declares the triangle class.
////////////////////////////////////////////////////////////////////////////////
#ifndef SHAPE_TRIANGLE_
#define SHAPE_TRIANGLE_

#include "pbj/_pbj.h"
#include "pbj/gfx/shape.h"
#include "pbj/_math.h"
#include "pbj/gfx/texture.h"

namespace pbj
{
namespace gfx
{

	class ShapeTriangle : public Shape
	{
	public:
		ShapeTriangle() {}
		~ShapeTriangle() {}

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