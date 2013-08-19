//inteface for shapes
#ifndef SHAPE_H_
#define SHAPE_H_

#include "pbj/_pbj.h"

namespace pbj
{
namespace gfx
{
	class Shape
	{
	public:
		virtual void draw(bool hasTexture) = 0;
	};
}
}
#endif