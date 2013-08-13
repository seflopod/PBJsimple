#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "pbj/_pbj.h"
#include "pbj/_math.h"
#include "pbj/gfx/texture.h"

namespace pbj
{
namespace gfx
{
	class Material
	{
	public:
		Material();
		~Material();

		GLuint getTextureId() const;
		void setTextureId(GLuint);


	private:
		GLuint _texId;
		color4 _color;
	};
} //namespace gfx
} //namespace pbj

#endif