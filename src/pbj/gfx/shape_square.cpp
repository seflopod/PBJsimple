#ifndef SHAPE_SQUARE_H_
#include "pbj/gfx/shape_square.h"
#endif

namespace pbj
{
namespace gfx
{
	void ShapeSquare::draw()
	{
		GLfloat curColor[4];
		glGetFloatv(GL_CURRENT_COLOR, curColor);
		draw(0, color4((F32)curColor[0], (F32)curColor[1], (F32)curColor[2], (F32)curColor[3]));
	}

	void ShapeSquare::draw(GLuint texId)
	{
		GLfloat curColor[4];
		glGetFloatv(GL_CURRENT_COLOR, curColor);
		draw(texId, color4((F32)curColor[0], (F32)curColor[1], (F32)curColor[2], (F32)curColor[3]));
	}

	void ShapeSquare::draw(color4 color)
	{
		draw(0, color);
	}

	void ShapeSquare::draw(GLuint texId, color4 color)
	{
		//probably unnecessary habit
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
		glEnable(GL_CULL_FACE);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);

		GLfloat curColor[4];
		glGetFloatv(GL_CURRENT_COLOR, curColor);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texId);

		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
		glBegin(GL_QUADS);
			glColor4f(color.r, color.g, color.b, color.a);
			glTexCoord2d(0.0, 0.0); glVertex2f(0.0f, 0.0f);
			glTexCoord2d(1.0, 0.0); glVertex2f(1.0f, 0.0f);
			glTexCoord2d(1.0, 1.0); glVertex2f(1.0f, 1.0f);
			glTexCoord2d(0.0, 1.0); glVertex2f(0.0f, 1.0f);
			glColor4fv(curColor);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}

	ShapeSquare::ShapeSquare() {}
	ShapeSquare::~ShapeSquare() {}
} //namespace gfx
} //namespace pbj
