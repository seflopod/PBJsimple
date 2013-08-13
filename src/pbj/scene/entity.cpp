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
		_rigidbody(nullptr),
		_player(nullptr),
        _transform(this)
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
	
	_transformCallbackId = U32(-1);
	_sceneId = 0;
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
	delete _rigidbody;
	delete _player;

	_rigidbody = nullptr;
	_player = nullptr;
	_initialized = false;
}

void Entity::update(F32 dt)
{
	if(_rigidbody)
		_rigidbody->updateOwnerTransform();
	
	if(_player)
	{
		if(!_player->isThrusting())
			_player->regenFuel();
		
		if(_player->reloading())
			_player->stepReloadTimer(dt);

		if(_player->fireOnCooldown())
			_player->stepFireTimer(dt);


		//std::cerr << _player->getFuelRemaining() << std::endl;
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

	glPushMatrix();
		glTranslatef(glmPos.x, glmPos.y, 0.0f);
		glRotatef(glmRot, 0, 0, 1);
		glScalef(glmSca.x, glmSca.y, 1.0f);
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

void Entity::addRigidbody(Rigidbody::BodyType bodyType, b2World* world)
{
	if(!_rigidbody)
	{
		vec2 scale = _transform.getScale();
		vec2 pos = _transform.getPosition();
		b2PolygonShape shape;

		shape.SetAsBox(scale.x/2, scale.y/2, b2Vec2_zero,
						_transform.getRotation());

		switch(_type)
		{
		case Player:
			_rigidbody = new Rigidbody(bodyType, pos, shape, world, 100.0f/(scale.x*scale.y), 0.0f, this);
			_rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::Player);
			break;
		case Terrain:
			_rigidbody = new Rigidbody(bodyType, pos, shape, world, 100.0f/(scale.x*scale.y), 0.0f, this);
			_rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::Terrain);
			break;
		case SpawnPoint:
			_rigidbody = new Rigidbody(bodyType, pos, shape, world, 1.0f/(scale.x*scale.y), 0.0f, this);
			_rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::SpawnPoint);
			break;
		case Bullet:
			_rigidbody = new Rigidbody(bodyType, pos, shape, world, 0.01f/(scale.x*scale.y), 0.0f, this);
		default:
			_rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::Other);
			break;
		}
		//_rigidbody->setTransform(b2Vec2(pos.x, pos.y),b2Vec2(1.0f, 1.0f),_transform.getRotation());
	}
}

Rigidbody* Entity::getRigidbody() const
{
	return _rigidbody;
}

void Entity::addPlayerComponent()
{
	if(!_player)
	{
		_player = new PlayerComponent(PlayerStats(), this);
	}
}

PlayerComponent* Entity::getPlayerComponent() const
{
	return _player;
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

void Entity::enableDraw()
{
    _drawable = true;
}


void Entity::disableDraw()
{
    _drawable = false;
}

bool Entity::isDrawable() const
{
    return _drawable;
}
