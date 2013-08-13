////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\src\pbj\gfx\shape_triangle.cpp
///
/// \brief	Implements the triangle class.
////////////////////////////////////////////////////////////////////////////////
#ifndef SHAPE_TRIANGLE_
#include "pbj\gfx\shape_triangle.h"
#endif

namespace pbj
{
namespace gfx
{

////////////////////////////////////////////////////////////////////////////////
/// \brief	Basic constructor / destructor
///
/// \author	Josh Douglas
/// \date	2013-08-13
////////////////////////////////////////////////////////////////////////////////
ShapeTriangle::ShapeTriangle()
{}

ShapeTriangle::~ShapeTriangle()
{}

////////////////////////////////////////////////////////////////////////////////
/// \brief	Default draw function
///
/// \author	Josh Douglas
/// \date	2013-08-13
////////////////////////////////////////////////////////////////////////////////
void ShapeTriangle::draw()
{
    GLfloat curColor[4];
    glGetFloatv(GL_CURRENT_COLOR, curColor);
    draw(0, color4((F32)curColor[0], (F32)curColor[1], (F32)curColor[2], (F32)curColor[3]));
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	draw function that takes in 1 params
///
/// \author	Josh Douglas
/// \date	2013-08-13
////////////////////////////////////////////////////////////////////////////////
void ShapeTriangle::draw(GLuint texId)
{
    GLfloat curColor[4];
    glGetFloatv(GL_CURRENT_COLOR, curColor);
    draw(texId, color4((F32)curColor[0], (F32)curColor[1], (F32)curColor[2], (F32)curColor[3]));
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	draw function that takes in 1 params
///
/// \author	Josh Douglas
/// \date	2013-08-13
////////////////////////////////////////////////////////////////////////////////
void ShapeTriangle::draw(color4 color)
{
    draw(0, color);
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	draw function that takes in 2 params
///
/// \author	Josh Douglas
/// \date	2013-08-13
////////////////////////////////////////////////////////////////////////////////
void ShapeTriangle::draw(GLuint texId, color4 color)
{
    //probably unnecessary habit
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

	//So many color calls!!!!
    //GLfloat curColor[4];
    //glGetFloatv(GL_CURRENT_COLOR, curColor);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texId);
	//glScalef(10.0f, 10.0f, 1.0f);
    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    glBegin(GL_TRIANGLES);
        //glColor4f(color.r, color.g, color.b, color.a);
		glColor4f(0.0, 0.0, 1.0, 1.0);
        glTexCoord2d(0.5, 0.0); glVertex2f(0.0f, 0.5f);
        glTexCoord2d(1.0, 0); glVertex2f(-0.325f, 0.0f);
        glTexCoord2d(0.0, 1.0); glVertex2f(0.325f, 0.0f);
        //glColor4fv(curColor);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

} // namespace gfx
} // namespace pbj