////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\include\pbj\gfx\shape_square.h
///
/// \brief	Declares the square class.
////////////////////////////////////////////////////////////////////////////////
#ifndef SHAPE_SQUARE_H_
#define SHAPE_SQUARE_H_

#include "pbj/_pbj.h"
#include "pbj/gfx/shape.h"
#include "pbj/_math.h"
#include "pbj/_gl.h"

namespace pbj
{
namespace gfx
{
	class ShapeSquare : public Shape
	{
	public:
		ShapeSquare() {}

		~ShapeSquare() {}

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