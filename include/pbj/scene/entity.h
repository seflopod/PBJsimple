////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJsimple\include\pbj\scene\entity.h
///
/// \brief	Declares the entity class.
////////////////////////////////////////////////////////////////////////////////
#ifndef ENTITY_H_
#define ENTITY_H_

#include <Box2D/Box2D.h>

#include "pbj/_pbj.h"
#include "pbj/gfx/texture.h"
#include "pbj/gfx/shape_square.h"
#include "pbj/gfx/shape_triangle.h"
#include "pbj/gfx/material.h"
#include "pbj/physics/rigidbody.h"
#include "pbj/scene/transform.h"
#include "pbj/scene/player_component.h"
#include "pbj/scene/ai_component.h"
#include "pbj/scene/bullet_component.h"
#include "pbj/audio/audio_source.h"
#include "pbj/audio/audio_listener.h"
#include "pbj/sw/sandwich.h"
#include "be/id.h"

#include <memory>

using std::unique_ptr;
using pbj::gfx::ComponentCallback;
using pbj::gfx::Texture;
using pbj::gfx::Shape;
using pbj::gfx::ShapeSquare;
using pbj::gfx::ShapeTriangle;
using pbj::gfx::Material;
using pbj::physics::Rigidbody;
using pbj::audio::AudioSource;
using pbj::audio::AudioListener;
using be::Id;

namespace pbj {
namespace scene {

////////////////////////////////////////////////////////////////////////////
/// \class	Entity
///
/// \brief	Entity keeps track of all data relating to an object within a
/// 		scene.
///
/// \author	Peter Bartosch
/// \date	2013-08-05
////////////////////////////////////////////////////////////////////////////
class Entity
{
public:
	////////////////////////////////////////////////////////////////////////
	/// \enum EntityType
	///
	/// \brief Values that represent EntityType.
	////////////////////////////////////////////////////////////////////////
	enum EntityType
	{
		Terrain = 0x01,
		Player = 0x02,
		SpawnPoint = 0x04,
		Bullet = 0x08
	};

	Entity();
	~Entity();
		
	void update(F32);
	void draw();
		
	//accessors, these will expand as the class gains more component
	//possiblities
	Transform& getTransform();
	void setTransform(const Transform&);
		
	Shape* getShape() const;
	void setShape(Shape*);

	const Material* getMaterial();
	void setMaterial(const Material*);

	GLuint getTextureId() const;
	void setTextureId(const GLuint);
		
	void addRigidbody(Rigidbody::BodyType, b2World*);
	Rigidbody* getRigidbody() const;

	void addPlayerComponent(Id);
	PlayerComponent* getPlayerComponent() const;

	void addAIComponent();
	AIComponent* getAIComponent() const;
	U32 getSceneId() const;
	void setSceneId(U32);

	void addBulletComponent();
	BulletComponent* getBulletComponent() const;

	void addAudioListener();
	AudioListener* getAudioListener() const;

	void addAudioSource();
	AudioSource* getAudioSource() const;

	EntityType getType() const;
	void setType(EntityType);


	bool isDrawable() const;
	void enableDraw();
	void disableDraw();

	bool isEnabled() const;
	void enable();
	void disable();

private:
    bool _drawable;
	bool _enabled;
	//bool _toDisable;
	//bool _toEnable;

	U32 _transformCallbackId;
	U32 _sceneId;
	EntityType _type;

	//components
	Transform _transform;
	unique_ptr<Shape> _shape;
	const Material* _material;    ///< Not a unique ptr because the entity does not own the material object (ResourceManager does)
	unique_ptr<Rigidbody> _rigidbody;
	unique_ptr<PlayerComponent> _player;
	unique_ptr<AIComponent> _ai;
	unique_ptr<BulletComponent> _bullet;
	unique_ptr<AudioSource> _src;
	unique_ptr<AudioListener> _listener;

	Entity(const Entity&);
	void operator=(const Entity&);
};

unique_ptr<Entity> loadEntity(sw::Sandwich& sandwich, const Id& map_id, const Id& entity_id);
void saveEntity(const Id& sandwich_id, const Id& map_id);

} //namespace pbj::scene
} //namespace pbj

#endif
