////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\src\pbj\scene\entity.cpp
///
/// \brief	Implements the entity class.
////////////////////////////////////////////////////////////////////////////////
#ifndef ENTITY_H_
#include "pbj/scene/entity.h"
#endif

#include <iostream>

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
Entity::Entity() :
		_rigidbody(nullptr)
{
	_initialized = false;

	_transform = Transform();
	_transformCallbackId = U32(-1);
	
	_textureId = 0;
	_drawable = false;
	_initialized = true;
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
	{
		//other destruction code goes here
		delete _rigidbody;
		_rigidbody = nullptr;
		_initialized = false;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	draws this object.
///
/// \author	Peter Bartosch / Josh Douglas
/// \date	2013-08-08
////////////////////////////////////////////////////////////////////////////////
void Entity::draw()
{
	color = color4(1.0f, 0.0f, 1.0f, 1.0f);
	vec2 glmPos = _transform.getPosition();
	F32 glmRot = _transform.getRotation();
	vec2 glmSca = _transform.getScale();
	GLfloat pos[2] = { glmPos.x, glmPos.y };
	GLfloat sca[2] = { glmSca.x, glmSca.y };

	glPushMatrix();
		glTranslatef(pos[0], pos[1], 0.0f);
		glRotatef(glmRot, 0, 0, 1);
		glScalef(sca[0], sca[1], 1.0f);
		//if colors are being done, use material.h  for now
		//this solid color works with no textures loaded.
		ShapeSquare::draw(_textureId, color);
	glPopMatrix();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Transform* Entity::getTransform() const
///
/// \brief	Gets the transform.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	null if it fails, else the transform.
////////////////////////////////////////////////////////////////////////////////
Transform* Entity::getTransform()
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

void Entity::addRigidbody(Rigidbody::BodyType bodyType, b2World* world)
{
	vec2 scale = _transform.getScale();
	vec2 pos = _transform.getPosition();
	b2PolygonShape shape;
	shape.SetAsBox(scale.x, scale.y, b2Vec2(pos.x, pos.y), _transform.getRotation());

	_rigidbody = new Rigidbody(bodyType, &shape, world, this);
	switch(_type)
	{
	case Player:
		_rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::Player);
		break;
	case Terrain:
		_rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::Terrain);
		break;
	case SpawnPoint:
		_rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::SpawnPoint);
		break;
	default:
		_rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::Other);
		break;
	}
}

Rigidbody* Entity::getRigidbody() const
{
	return _rigidbody;
}

U32 Entity::getSceneId() const
{
	return _sceneId;
}

void Entity::setSceneId(U32 id)
{
	_sceneId = id;
}

Entity::EntityType Entity::getType() const
{
	return _type;
}

void Entity::setType(EntityType et)
{
	_type = et;
	if(_rigidbody != nullptr)
	{
		switch(_type)
		{
		case Player:
			_rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::Player);
			break;
		case Terrain:
			_rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::Terrain);
			break;
		case SpawnPoint:
			_rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::SpawnPoint);
			break;
		default:
			_rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::Other);
			break;
		}
	}
}

bool Entity::isDrawable() const
{
	return _drawable;
}

void Entity::enableDraw()
{
	_drawable = true;
}

void Entity::disableDraw()
{
	_drawable = false;
}