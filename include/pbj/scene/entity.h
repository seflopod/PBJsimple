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
#include "pbj/transform.h"
#include "pbj/gfx/texture.h"
#include "pbj/gfx/shape_square.h"
#include "pbj/gfx/shape_triangle.h"
#include "pbj/gfx/material.h"
#include "pbj/physics/rigidbody.h"
#include "pbj/scene/player_component.h"
#include "pbj/scene/ai_component.h"

using pbj::gfx::ComponentCallback;
using pbj::gfx::Texture;
using pbj::gfx::Shape;
using pbj::gfx::ShapeSquare;
using pbj::gfx::ShapeTriangle;
using pbj::gfx::Material;
using pbj::physics::Rigidbody;

namespace pbj
{
namespace scene
{
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
		
		//intialization and destruction funcs
		void init();
		void destroy();
		
		void update(F32);
		void draw();
		
		//accessors, these will expand as the class gains more component
		//possiblities
		Transform* getTransform();
		void setTransform(const Transform&);
		
		Shape* getShape() const;
		void addShape(Shape*);

		std::shared_ptr<Material> getMaterial();
		void addMaterial(std::shared_ptr<Material>);

		GLuint getTextureId() const;
		void setTextureId(const GLuint);
		
		void addRigidbody(Rigidbody::BodyType, b2World*);
		Rigidbody* getRigidbody() const;

		void addPlayerComponent();
		PlayerComponent* getPlayerComponent() const;

		void addAIComponent();
		AIComponent* getAIComponent() const;
		U32 getSceneId() const;
		void setSceneId(U32);

		EntityType getType() const;
		void setType(EntityType);


		bool isDrawable() const;
		void enableDraw();
		void disableDraw();

		bool isEnabled() const;
		void enable();
		void disable();

	private:
		bool _initialized;
				
        bool _drawable;
		bool _enabled;
		//bool _toDisable;
		//bool _toEnable;

		U32 _transformCallbackId;
		U32 _sceneId;
		EntityType _type;

		//components
		Transform _transform;
		std::unique_ptr<Shape> _shape;
		std::shared_ptr<Material> _material;
		Rigidbody* _rigidbody;
		PlayerComponent* _player;
		std::unique_ptr<AIComponent> _ai;
		Entity(const Entity&);
		void operator=(const Entity&);
	};
} //namespace pbj::scene
} //namespace pbj
#endif
