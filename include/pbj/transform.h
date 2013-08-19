///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/transform.h
/// \author Peter Bartosch / Josh Douglas
/// \date   2013-07-18
/// \brief  A class for representing the transformation of an object.
/// \details This class is mostly a wrapper for glm calls.  It stores the
///         position, rotation, and scale of an object.

#ifndef PBJ_TRANSFORM_H_
#define PBJ_TRANSFORM_H_

#include "pbj/_pbj.h"
#include "pbj/_math.h"

namespace pbj {

class Transform
{
public:
	Transform(void*);
	~Transform();

	void rotate(F32);

	void move(F32, F32);
	void move(const vec2&);

	const vec2& getPosition() const;
	void setPosition(F32, F32);
	void setPosition(const vec2&);

	F32 getRotation() const;
	void setRotation(F32 rotation);

	const vec2& getScale() const;
	void setScale(F32, F32);
	void setScale(const vec2&);

	//mat4 getMatrix() const;

	void* getOwner();

	void updateOwnerRigidbody();
private:
	void* _owner;
	vec2 _position;	///< The position.
	F32 _rotation;	///< Rotation stored as a quaternion.
	vec2 _scale;	///< Scale.
};

} //namespace pbj

#endif
