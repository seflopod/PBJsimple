////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\include\pbj\gfx\shape_square.h
///
/// \brief	Declares the square class.
////////////////////////////////////////////////////////////////////////////////
#ifndef SHAPE_SQUARE_H_
#define SHAPE_SQUARE_H_

#include "pbj/_pbj.h"
#include "pbj/_math.h"
#include "pbj/_gl.h"

namespace pbj
{
namespace gfx
{
	//I don't think this is a terrible way to do the drawing of simple shapes, but
	//I do think that if we start expanding to circles, triangles or whatever that
	//we should seriously consider making a shape template instead of abstract.
	//just thinking about singleton stuff, because based on the way this has gone
	//there is clearly no need to continue to create instances of the class.
	class ShapeSquare
	{
	public:
		//the overloads are most likely unnecessary but harmless.
		//once I started making them I just couldn't stop.
		//I don't have an overloading problem, people just don't understand
		static void draw();
		static void draw(GLuint);
		static void draw(color4);
		static void draw(GLuint, color4);

		~ShapeSquare();
	private:
		ShapeSquare();
	};
} //namespace gfx
} //namspace pbj
#endif