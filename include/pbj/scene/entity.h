////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\include\pbj\scene\entity.h
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
#include "pbj/physics/rigidbody.h"
#include "pbj/scene/player_component.h"
//too lazy to check which if these actually need to be a part of this
//I figure it's worth including the possibility that an entity might be text
//though for our current purposes this might be a bit overboard.
#include "pbj/gfx/texture_font.h"
#include "pbj/gfx/texture_font_character.h"

using pbj::gfx::ComponentCallback;
using pbj::gfx::Texture;
using pbj::gfx::ShapeSquare;
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
			SpawnPoint = 0x04
		};

		Entity();
		
		~Entity();
		
		//intialization and destruction funcs
		void init();
		void destroy();
		
		void draw();
		
		//accessors, these will expand as the class gains more component
		//possiblities
		Transform* getTransform();
		void setTransform(const Transform&);
		
		GLuint getTextureId() const;
		void setTextureId(const GLuint);
		
		void addRigidbody(Rigidbody::BodyType, b2World*);
		Rigidbody* getRigidbody() const;

		void addPlayerComponent();
		PlayerComponent* getPlayerComponent() const;

		U32 getSceneId() const;
		void setSceneId(U32);

		EntityType getType() const;
		void setType(EntityType);


		bool isDrawable() const;
		void enableDraw();
		void disableDraw();
		color4 color;


	private:
		bool _initialized;
				
        bool _drawable;

		U32 _transformCallbackId;
		U32 _sceneId;
		EntityType _type;

		//components
		Transform _transform;
		GLuint _textureId;
		Rigidbody* _rigidbody;
		PlayerComponent* _player;

		Entity(const Entity&);
		void operator=(const Entity&);
	};
} //namespace pbj::scene
} //namespace pbj
#endif
