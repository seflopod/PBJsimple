////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJgame\include\pbj\scene\entity.h
///
/// \brief	Declares the entity class.
////////////////////////////////////////////////////////////////////////////////
#ifndef ENTITY_H_
#define ENTITY_H_

#include "pbj/_pbj.h"
#include "pbj/transform.h"
#include "pbj/gfx/texture.h"
#include "pbj/gfx/shape_square.h"
//too lazy to check which if these actually need to be a part of this
//I figure it's worth including the possibility that an entity might be text
//though for our current purposes this might be a bit overboard.
#include "pbj/gfx/texture_font.h"
#include "pbj/gfx/texture_font_character.h"

using pbj::gfx::ComponentCallback;
using pbj::gfx::Texture;
using pbj::gfx::ShapeSquare;

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
		Entity();
		
		~Entity();
		
		//intialization and destruction funcs
		void init();
		void destroy();
		
		void draw();
		
		//accessors, these will expand as the class gains more component
		//possiblities
		const Transform* getTransform() const;
		void setTransform(const Transform&);
		
		GLuint getTextureId() const;
		void setTextureId(const GLuint);
		
		
	private:
		bool _initialized;
				
		U32 _transformCallbackId;
		
		//components
		Transform _transform;
		GLuint _textureId;

		Entity(const Entity&);
		void operator=(const Entity&);
	};
} //namespace pbj::scene
} //namespace pbj
#endif
