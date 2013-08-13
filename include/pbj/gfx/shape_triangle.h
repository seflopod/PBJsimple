////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\include\pbj\gfx\shape_triangle.h
///
/// \brief	Declares the triangle class.
////////////////////////////////////////////////////////////////////////////////
#ifndef SHAPE_TRIANGLE_
#define SHAPE_TRIANGLE_

#include "pbj/_pbj.h"
#include "pbj/_math.h"
#include "pbj/gfx/texture.h"

namespace pbj
{
namespace gfx
{

class ShapeTriangle
{
public:
	ShapeTriangle();
	~ShapeTriangle();
	//basically a copy and past from the square class
	//just adjusted for a triangle
	static void draw();
	static void draw(GLuint);
	static void draw(color4);
	static void draw(GLuint, color4);
};

} // namespace gfx
} // namespace pbj
#endif