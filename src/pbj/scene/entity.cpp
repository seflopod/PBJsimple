////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\src\pbj\scene\entity.cpp
///
/// \brief	Implements the entity class.
////////////////////////////////////////////////////////////////////////////////
#ifndef ENTITY_H_
#include "pbj/scene/entity.h"
#endif

using namespace pbj;
using namespace pbj::scene;

////////////////////////////////////////////////////////////////////////////////
/// \fn	Entity::Entity()
///
/// \brief	Default constructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
Entity::Entity()
{
	_initialized = false;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Entity::~Entity()
///
/// \brief	Destructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
Entity::~Entity()
{
	if(_initialized)
		destroy();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::init()
///
/// \brief	Initialises this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////////
void Entity::init()
{
	_transform = Transform();
	_transformCallbackId = U32(-1);
	
	_textureId = 0;
	_initialized = true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::destroy()
///
/// \brief	Destroys this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
/// \details    Holdover from old version.  May or may not necessary down the
///             road.
////////////////////////////////////////////////////////////////////////////////
void Entity::destroy()
{
	_initialized = false;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::draw()
///
/// \brief	draws this object.
///
/// \author	Peter Bartosch / Josh Douglas
/// \date	2013-08-08
////////////////////////////////////////////////////////////////////////////////
void Entity::draw()
{
	vec2 glmPos = _transform.getPosition();
	F32 glmRot = _transform.getRotation();
	vec2 glmSca = _transform.getScale();
	GLfloat pos[3] = { glmPos.x, glmPos.y};
	GLfloat sca[2] = { glmSca.x, glmSca.y};

	glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2]);
		glRotatef(glmRot, 0, 0, 1);
		glScalef(sca[0], sca[1], sca[2]);
		//if colors are being done, use material.h  for now
		//this solid color works with no textures loaded.
		ShapeSquare::draw(_textureId, color4(0.0f,1.0f,0.0f,1.0f));
	glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	const Transform* Entity::getTransform() const
///
/// \brief	Gets the transform.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	null if it fails, else the transform.
////////////////////////////////////////////////////////////////////////////////
const Transform* Entity::getTransform() const
{
	if(_initialized)
		return &_transform;
	return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::setTransform(Transform transform)
///
/// \brief	Sets a transform.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \param	transform	The transform.
////////////////////////////////////////////////////////////////////////////////
void Entity::setTransform(const Transform& transform)
{
	if(!_initialized)
		init();
	_transform = transform;
}

GLuint Entity::getTextureId() const
{
	return _textureId;
}

void Entity::setTextureId(const GLuint newId)
{
	_textureId = newId;
}