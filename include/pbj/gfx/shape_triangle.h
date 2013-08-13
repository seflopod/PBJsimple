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
	static void draw();
	static void draw(GLuint);
	static void draw(color4);
	static void draw(GLuint, color4);
};

} // namespace gfx
} // namespace pbj
#endif