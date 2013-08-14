////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJsimple\src\pbj\scene\entity.cpp
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
		_shape(nullptr),
		_material(nullptr),
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
	_enabled = true;
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::update(F32 dt)
///
/// \brief	Updates using the given difference in time.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	dt	The difference in time.
/// 
/// \details This mostly checks firing states for the PlayerComponent.  It also
/// 		 uses the Rigidbody to make sure the Transform used for drawing is
/// 		 up-to-date.
////////////////////////////////////////////////////////////////////////////////
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
	vec2 glmPos = _transform.getPosition();
	F32 glmRot = _transform.getRotation();
	vec2 glmSca = _transform.getScale();
	GLuint texId = _material->getTextureId();
	color4 color = _material->getColor();
	//probably unnecessary habit
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	//save the current colour to put it back when we're done
	GLfloat curColor[4];
	glGetFloatv(GL_CURRENT_COLOR, curColor);

	if(texId)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texId);
		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
	}
	
	glPushMatrix();
		glTranslatef(glmPos.x, glmPos.y, 0.0f);
		glRotatef(glmRot, 0, 0, 1);
		glScalef(glmSca.x, glmSca.y, 1.0f);
		glColor4f(color.r, color.g, color.b, color.a);
		_shape->draw((texId!=0));
	glPopMatrix();
	glColor4fv(curColor);

	if(texId)
		glDisable(GL_TEXTURE_2D);
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	Shape* Entity::getShape() const
///
/// \brief	Gets the shape used by the Entity.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	null if the shape does not exist; a pointer to the Shape otherwise.
////////////////////////////////////////////////////////////////////////////////
Shape* Entity::getShape() const
{
	return _shape.get();
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::addShape(Shape* shape)
///
/// \brief	Adds a Shape.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param [in]	shape	A pointer to the Shape to use.
/// \details Any class the uses the Shape interface (ShapeSquare, ShapeTriangle)
/// 		 is acceptable.  This is what will be drawn.
////////////////////////////////////////////////////////////////////////////////
void Entity::addShape(Shape* shape)
{
	if(_shape != nullptr)
	{
		_shape.release();
	}
	_shape.reset(shape);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	std::shared_ptr<Material> Entity::getMaterial()
///
/// \brief	Gets the material.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	A shared_ptr to the Material.
////////////////////////////////////////////////////////////////////////////////
std::shared_ptr<Material> Entity::getMaterial()
{
	return _material;
}

/// \fn	void Entity::addMaterial(std::shared_ptr<Material> material)
///
/// \brief	Adds a material.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	material	A shared_ptr to the Material for this Entity to use.
/// \details Since a sinigle Material can be used for multiple Entitys it makes
/// 		 the most sense to use a pointer.  I chose to use a shared_ptr
/// 		 because I'm lazy and don't want to keep track of the pointer
/// 		 myself.
////////////////////////////////////////////////////////////////////////////////
void Entity::addMaterial(std::shared_ptr<Material> material)
{
	if(_material.get()!=nullptr)
	{
		_material.reset<Material>(nullptr);
	}
	_material = material;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::addRigidbody(Rigidbody::BodyType bodyType,
/// 	b2World* world)
///
/// \brief	Adds a rigidbody to the Entity.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	bodyType	 	Type of the body.
/// \param [in]	world	The Box2D world that holds the Rigidbody.
/// \details This will create a Rigidbody based on the type of Entity that is
/// 		 using it.  Things like shape, weight and restitution (bounciness)
/// 		 are all passed to the Rigidbody class to create a new one for this
/// 		 particular Entity.  This is rather simple and therefore does not
/// 		 use multiple fixtures for making complex shapes.  Simple boxes,
/// 		 polygons and circles are used instead.
////////////////////////////////////////////////////////////////////////////////
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
			_rigidbody = new Rigidbody(bodyType, pos, shape, world, 100.0f/(scale.x*scale.y), 0.0f, 0.5f,this);
			_rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::Player);
			break;
		case Terrain:
			_rigidbody = new Rigidbody(bodyType, pos, shape, world, 100.0f/(scale.x*scale.y), 0.0f, 0.5f,this);
			_rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::Terrain);
			break;
		case SpawnPoint:
			_rigidbody = new Rigidbody(bodyType, pos, shape, world, 1.0f/(scale.x*scale.y), 0.0f, 1.0f, this);
			_rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::SpawnPoint);
			break;
		case Bullet:
			_rigidbody = new Rigidbody(bodyType, pos, shape, world, 0.01f/(scale.x*scale.y), 0.5f, 0.1f,this);
		default:
			_rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::Other);
			break;
		}
		//_rigidbody->setTransform(b2Vec2(pos.x, pos.y),b2Vec2(1.0f, 1.0f),_transform.getRotation());
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Rigidbody* Entity::getRigidbody() const
///
/// \brief	Gets a pointer to the Rigidbody.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	null if it none exists, else the rigidbody.
////////////////////////////////////////////////////////////////////////////////
Rigidbody* Entity::getRigidbody() const
{
	return _rigidbody;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::addPlayerComponent()
///
/// \brief	Adds player component.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
////////////////////////////////////////////////////////////////////////////////
void Entity::addPlayerComponent()
{
	if(!_player)
	{
		_player = new PlayerComponent(PlayerStats(), this);
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	PlayerComponent* Entity::getPlayerComponent() const
///
/// \brief	Gets PlayerComponent.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	null if none exists, else the player component.
////////////////////////////////////////////////////////////////////////////////
PlayerComponent* Entity::getPlayerComponent() const
{
	return _player;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	U32 Entity::getSceneId() const
///
/// \brief	Gets scene identifier.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	The scene identifier.
////////////////////////////////////////////////////////////////////////////////
U32 Entity::getSceneId() const
{
	return _sceneId;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::setSceneId(U32 id)
///
/// \brief	Sets scene identifier.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	id	The identifier.
////////////////////////////////////////////////////////////////////////////////
void Entity::setSceneId(U32 id)
{
	_sceneId = id;
}

Entity::EntityType Entity::getType() const
{
	return _type;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::setType(EntityType et)
///
/// \brief	Sets a type.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	et	The EntityType to use.
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::enableDraw()
///
/// \brief	Enables drawing.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
////////////////////////////////////////////////////////////////////////////////
void Entity::enableDraw()
{
    _drawable = true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::disableDraw()
///
/// \brief	Disables drawing.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
////////////////////////////////////////////////////////////////////////////////
void Entity::disableDraw()
{
    _drawable = false;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool Entity::isDrawable() const
///
/// \brief	Query if this object is drawable.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	true if drawable, false if not.
////////////////////////////////////////////////////////////////////////////////
bool Entity::isDrawable() const
{
    return _drawable;
}


bool Entity::isEnabled() const
{
	return _enabled;
}

void Entity::enable()
{
	if(_shape.get())
		_drawable = true;
	if(_rigidbody)
		_rigidbody->setActive(true);

	_enabled = true;
}
void Entity::disable()
{
	_drawable = false;
	if(_rigidbody)
		_rigidbody->setActive(false);

	_enabled = false;
}
