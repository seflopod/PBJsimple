////////////////////////////////////////////////////////////////////////////////
/// \file   Z:\Documents\PBJsimple\src\pbj\scene\entity.cpp
///
/// \brief  Implements the entity class.
////////////////////////////////////////////////////////////////////////////////
#ifndef ENTITY_H_
#include "pbj/scene/entity.h"
#endif

#include "be/bed/transaction.h"
#include "pbj/sw/sandwich_open.h"

#include <iostream>
#include "pbj/game.h"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to load an entity from a sandwich.
#define PBJ_SCENE_ENTITY_SQL_LOAD "SELECT entity_type, rotation, " \
            "pos_x, pos_y, scale_x, scale_y, material_sw_id, material_id " \
            "FROM sw_map_entities WHERE map_id = ? AND entity_id = ?"

///////////////////////////////////////////////////////////////////////////////
/// \brief  SQL statement to save an entity to a sandwich.
#define PBJ_SCENE_ENTITY_SQL_SAVE "INSERT INTO sw_map_entities " \
            "(map_id, entity_id, entity_type, rotation, " \
            "pos_x, pos_y, scale_x, scale_y, material_sw_id, material_id) " \
            "VALUES (?,?,?,?,?,?,?,?,?,?)"

#ifdef BE_ID_NAMES_ENABLED
#define PBJ_SCENE_ENTITY_SQLID_LOAD PBJ_SCENE_ENTITY_SQL_LOAD
#define PBJ_SCENE_ENTITY_SQLID_SAVE PBJ_SCENE_ENTITY_SQL_SAVE
#else
// TODO: precalculate ids.
#define PBJ_SCENE_ENTITY_SQLID_LOAD PBJ_SCENE_ENTITY_SQL_LOAD
#define PBJ_SCENE_ENTITY_SQLID_SAVE PBJ_SCENE_ENTITY_SQL_SAVE
#endif

namespace pbj {
namespace scene {

////////////////////////////////////////////////////////////////////////////////
/// \fn	Entity::Entity()
///
/// \brief	Default constructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
Entity::Entity() :
		_shape(nullptr),
		_material(nullptr),
		_rigidbody(nullptr),
		_player(nullptr),
        _transform(this),
		_ai(nullptr)
{
    _transformCallbackId = U32(-1);
	_sceneId = 0;
	_enabled = true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Entity::~Entity()
///
/// \brief	Destructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
Entity::~Entity()
{
	_shape.reset();
	_rigidbody.reset();
	_player.reset();
	_ai.reset();
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
void Entity::update(F32 dt)
{
    if(_rigidbody)
        _rigidbody->updateOwnerTransform();
	
    if(_ai.get())
        _ai->update(dt);

    if(_player)
    {
        if(!_player->isThrusting())
            _player->regenFuel();
		
        if(_player->reloading())
            _player->stepReloadTimer(dt);

        if(_player->fireOnCooldown())
            _player->stepFireTimer(dt);
    }

    //If a bullet isn't moving fast enough, make it disappear
    if(_type == Bullet && _rigidbody && glm::length2(_rigidbody->getVelocity()) < 16.0f)
        Game::instance()->disableBullet(this);

    //If a player moves too far out of range, kill it.
    if(_type == Player && _transform.getPosition().y - _transform.getScale().y/2 < -Game::grid_height)
    {
        _player->setTimeOfDeath(glfwGetTime());
        Game::instance()->respawnPlayer(this);
    }

    //Make sure the camera is doing as it needs to do.
    if(_type == Camera)
    {
        if(_camera)
            _camera->update(dt);
        if(_listener)
        {
            _listener->updatePosition();
            _listener->updateVelocity();
        }
    }
}
////////////////////////////////////////////////////////////////////////////////
/// \brief	draws this object.
///
/// \author	Peter Bartosch / Josh Douglas / Ben Crist
/// \date	2013-08-08
////////////////////////////////////////////////////////////////////////////////
void Entity::draw()
{
    vec2 glmPos = _transform.getPosition();
    F32 glmRot = _transform.getRotation();
    vec2 glmSca = _transform.getScale();

    if (_material)
        _material->use();
    else
        Texture::disable();

    glPushMatrix();
        glTranslatef(glmPos.x, glmPos.y, 0.0f);
        glRotatef(glmRot, 0, 0, 1);
        glScalef(glmSca.x, glmSca.y, 1.0f);
        _shape->draw((_material->getTexture() != nullptr));
    glPopMatrix();
}

#pragma region components
////////////////////////////////////////////////////////////////////////////////
/// \fn	Transform* Entity::getTransform() const
///
/// \brief	Gets the transform.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
///
/// \return	the transform.
////////////////////////////////////////////////////////////////////////////////
Transform& Entity::getTransform() { return _transform; }

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
void Entity::setTransform(const Transform& transform) { _transform = transform; }

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
Shape* Entity::getShape() const { return _shape.get(); }

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::setShape(Shape* shape)
///
/// \brief	Sets the shape of the entity.
///
/// \author	Peter Bartosch / Ben Crist
/// \date	2013-08-13
///
/// \param [in]	shape	A pointer to the Shape to use.
/// \details Any class the uses the Shape interface (ShapeSquare, ShapeTriangle)
/// 		 is acceptable.  This is what will be drawn.
////////////////////////////////////////////////////////////////////////////////
void Entity::setShape(Shape* shape) { _shape.reset(shape); }

////////////////////////////////////////////////////////////////////////////////
/// \brief	Gets the entity's material.
///
/// \author	Peter Bartosch / Ben Crist
/// \date	2013-08-13
///
/// \return	A pointer to the Material.
////////////////////////////////////////////////////////////////////////////////
const Material* Entity::getMaterial() { return _material; }

////////////////////////////////////////////////////////////////////////////////
/// \brief	Adds a material.
///
/// \author	Peter Bartosch / Ben Crist
/// \date	2013-08-13
///
/// \param	material A pointer to a material object.  The object's lifetime
///         must be at least as long as this entity's.  In the normal usage,
///         a ResourceManager will manage the lifetime of the material object.
////////////////////////////////////////////////////////////////////////////////
void Entity::setMaterial(const Material* material) { _material = material; }

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::addRigidbody(Rigidbody::BodyType bodyType,
/// 	b2World* world)
///
/// \brief	Adds a rigidbody to the Entity.
///
/// \author	Peter Bartosch / Ben Crist
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

        Rigidbody* rigidbody = nullptr;

        switch(_type)
        {
        case Player:
        {
            shape.SetAsBox(scale.x/2, scale.y/2, b2Vec2_zero,
            _transform.getRotation());
            _rigidbody.reset(new Rigidbody(bodyType, pos, shape, world, 100.0f/(scale.x*scale.y), 0.0f, 0.5f,this));
            _rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::Player);
            break;
        }
        case Terrain:
        {
            shape.SetAsBox(scale.x/2, scale.y/2, b2Vec2_zero,
            _transform.getRotation());
            _rigidbody.reset(new Rigidbody(bodyType, pos, shape, world, 100.0f/(scale.x*scale.y), 0.0f, 0.5f,this));
            _rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::Terrain);
            break;
        }
        case SpawnPoint:
        {
            shape.SetAsBox(scale.x/2, scale.y/2, b2Vec2_zero,
            _transform.getRotation());
            _rigidbody.reset(new Rigidbody(bodyType, pos, shape, world, 1.0f/(scale.x*scale.y), 0.0f, 1.0f, this));
            _rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::SpawnPoint);
            break;
        }
        case Bullet:
        {
            b2Vec2 verts[3];
            verts[0] = b2Vec2(-0.5f*scale.x, -0.433f*scale.y);
            verts[1] = b2Vec2(0.5f*scale.x, -0.433f*scale.y);
            verts[2] = b2Vec2(0.0f*scale.x, 0.433f*scale.y);
            shape.Set(verts,3);
            _rigidbody.reset(new Rigidbody(bodyType, pos, shape, world, 0.01f/(scale.x*scale.y), 0.5f, 0.1f,this));
        }
        default:
        {
            shape.SetAsBox(scale.x/2, scale.y/2, b2Vec2_zero,
            _transform.getRotation());
            _rigidbody.reset(new Rigidbody(bodyType, pos, shape, world, 1.0f/(scale.x*scale.y), 0.0f, 1.0f, this));
            _rigidbody->setCollisionGroup(Rigidbody::CollisionGroup::Other);
        break;
        }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	Rigidbody* Entity::getRigidbody() const
///
/// \brief	Gets a pointer to the Rigidbody.
///
/// \author	Peter Bartosch / Ben Crist
/// \date	2013-08-13
///
/// \return	null if it none exists, else the rigidbody.
////////////////////////////////////////////////////////////////////////////////
Rigidbody* Entity::getRigidbody() const { return _rigidbody.get(); }

////////////////////////////////////////////////////////////////////////////////
/// \fn	void Entity::addPlayerComponent()
///
/// \brief	Adds player component.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
////////////////////////////////////////////////////////////////////////////////
void Entity::addPlayerComponent(Id id)
{
	if(!_player)
		_player.reset(new PlayerComponent(id, PlayerStats(), this));
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
PlayerComponent* Entity::getPlayerComponent() const { return _player.get(); }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::addAIComponent()
///
/// \brief  Adds an AI component.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void Entity::addAIComponent() { _ai.reset(new AIComponent(this)); }

////////////////////////////////////////////////////////////////////////////////
/// \fn AIComponent* Entity::getAIComponent() const
///
/// \brief  Gets an AI component.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the AI component.
////////////////////////////////////////////////////////////////////////////////
AIComponent* Entity::getAIComponent() const { return _ai.get(); }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::addBulletComponent()
///
/// \brief  Adds bullet component.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void Entity::addBulletComponent() { _bullet.reset(new BulletComponent(this)); }

////////////////////////////////////////////////////////////////////////////////
/// \fn BulletComponent* Entity::getBulletComponent() const
///
/// \brief  Gets bullet component.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the bullet component.
////////////////////////////////////////////////////////////////////////////////
BulletComponent* Entity::getBulletComponent() const { return _bullet.get(); }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::addAudioListener()
///
/// \brief  Adds audio listener.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void Entity::addAudioListener() { _listener.reset(new AudioListener(this)); }

////////////////////////////////////////////////////////////////////////////////
/// \fn AudioListener* Entity::getAudioListener() const
///
/// \brief  Gets audio listener.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the audio listener.
////////////////////////////////////////////////////////////////////////////////
AudioListener* Entity::getAudioListener() const { return _listener.get(); }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::addAudioSource()
///
/// \brief  Adds audio source.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void Entity::addAudioSource() { _src.reset(new AudioSource(this)); }

////////////////////////////////////////////////////////////////////////////////
/// \fn AudioSource* Entity::getAudioSource() const
///
/// \brief  Gets audio source.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the audio source.
////////////////////////////////////////////////////////////////////////////////
AudioSource* Entity::getAudioSource() const { return _src.get(); }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Entity::addCamera()
///
/// \brief  Adds camera.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void Entity::addCamera() { _camera.reset(new CameraComponent(this)); }

////////////////////////////////////////////////////////////////////////////////
/// \fn CameraComponent* Entity::getCamera() const
///
/// \brief  Gets the camera.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the camera.
////////////////////////////////////////////////////////////////////////////////
CameraComponent* Entity::getCamera() const { return _camera.get(); }

#pragma endregion
////////////////////////////////////////////////////////////////////////////////
/// \fn	U32 Entity::getSceneId() const
///
/// \brief	Gets scene identifier.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	The scene identifier.
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
/// \brief	Sets the EntityType and adds the Rigidbody to the right collision
///         group.
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

////////////////////////////////////////////////////////////////////////////////
std::unique_ptr<Entity> loadEntity(sw::Sandwich& sandwich, const Id& map_id, const Id& entity_id)
{
    std::unique_ptr<Entity> value;

    try
    {
        db::StmtCache& cache = sandwich.getStmtCache();
        db::CachedStmt stmt = cache.hold(Id(PBJ_SCENE_ENTITY_SQLID_LOAD), PBJ_SCENE_ENTITY_SQL_LOAD);

        stmt.bind(1, map_id.value());
        stmt.bind(2, entity_id.value());
        if (stmt.step())
        {
            Entity::EntityType type = static_cast<Entity::EntityType>(stmt.getUInt(0));
            F32 rotation = float(stmt.getDouble(1));
            vec2 position = vec2(float(stmt.getDouble(2)), float(stmt.getDouble(3)));
            vec2 scale = vec2(float(stmt.getDouble(4)), float(stmt.getDouble(5)));

            const Material* material = nullptr;
            if (stmt.getType(7) != SQLITE_NULL)
            {
                sw::ResourceId material_id;
                material_id.sandwich = (stmt.getType(6) == SQLITE_NULL)
                                        ? sandwich.getId()
                                        : Id(stmt.getUInt64(6));
                material_id.resource = Id(stmt.getUInt64(7));
                material = &getEngine().getResourceManager().getMaterial(material_id);
            }

            value.reset(new Entity());
            value->setType(type);
            value->getTransform().setPosition(position);
            value->getTransform().setScale(scale);
            value->getTransform().setRotation(rotation);
            value->setMaterial(material);

            switch (type)
            {
                case Entity::Terrain:
                    value->setShape(new ShapeSquare());
                    /// TODO: fix physics world part of engine instead of scene?
                    value->addRigidbody(Rigidbody::BodyType::Static, Game::instance()->currentScene().getWorld());
                    value->enableDraw();
                    break;

                case Entity::SpawnPoint:
                    value->setShape(new ShapeSquare());
#ifndef PBJ_EDITOR
                    //disable this when not testing
                    //value->enableDraw();
	                value->disableDraw();
#else
                    // When in the editor, draw spawnpoints no matter what.
                    value->enableDraw();
#endif
                    break; 

                default:
                    break;
            }
        }
        else
            throw std::runtime_error("Entity not found!");
    }
    catch (const db::Db::error& err)
    {
        PBJ_LOG(VWarning) << "Database error while loading entity!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "     Map ID: " << map_id << PBJ_LOG_NL
                          << "  Entity ID: " << entity_id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_NL
                          << "        SQL: " << err.sql() << PBJ_LOG_END;
   }
   catch (const std::exception& err)
   {
      PBJ_LOG(VWarning) << "Exception while loading entity!" << PBJ_LOG_NL
                          << "Sandwich ID: " << sandwich.getId() << PBJ_LOG_NL
                          << "     Map ID: " << map_id << PBJ_LOG_NL
                          << "  Entity ID: " << entity_id << PBJ_LOG_NL
                          << "  Exception: " << err.what() << PBJ_LOG_END;
   }

    return value;
}

////////////////////////////////////////////////////////////////////////////////
void Entity::saveEntity(const Id& sandwich_id, const Id& map_id)
{
    try
    {
      std::shared_ptr<sw::Sandwich> sandwich = sw::openWritable(sandwich_id);
      if (!sandwich)
         throw std::runtime_error("Could not open sandwich for writing!");

      db::Db& db = sandwich->getDb();
      db::Transaction transaction(db, db::Transaction::Immediate);

      db::CachedStmt& stmt = sandwich->getStmtCache().hold(Id(PBJ_SCENE_ENTITY_SQLID_SAVE), PBJ_SCENE_ENTITY_SQL_SAVE);
      stmt.bind(1, map_id.value());
      stmt.bind(2, getSceneId());
      stmt.bind(3, getType());
      stmt.bind(4, getTransform().getRotation());
      stmt.bind(5, getTransform().getPosition().x);
      stmt.bind(6, getTransform().getPosition().y);
      stmt.bind(7, getTransform().getScale().x);
      stmt.bind(8, getTransform().getScale().y);
      if (_material)
      {
          stmt.bind(9, _material->getId().sandwich.value());
          stmt.bind(10, _material->getId().resource.value());
      }
      else
      {
          stmt.bind(9);
          stmt.bind(10);
      }
      stmt.step();

      transaction.commit();
   }
   catch (const db::Db::error& err)
   {
      PBJ_LOG(VWarning) << "Database error while saving scene!" << PBJ_LOG_NL
                       << "Sandwich ID: " << sandwich_id << PBJ_LOG_NL
                       << "     Map ID: " << map_id << PBJ_LOG_NL
                       << "  Entity ID: " << getSceneId() << PBJ_LOG_NL
                       << "  Exception: " << err.what() << PBJ_LOG_NL
                       << "        SQL: " << err.sql() << PBJ_LOG_END;
   }
   catch (const std::runtime_error& err)
   {
      PBJ_LOG(VWarning) << "Exception while saving scene!" << PBJ_LOG_NL
                       << "Sandwich ID: " << sandwich_id << PBJ_LOG_NL
                       << "     Map ID: " << map_id << PBJ_LOG_NL
                       << "  Entity ID: " << getSceneId() << PBJ_LOG_NL
                       << "  Exception: " << err.what() << PBJ_LOG_END;
   }
}

} // namespace pbj::scene
} // namespace pbj
