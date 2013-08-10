///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/transform.cpp
/// \author Peter Bartosch / Josh Douglas
/// \date   2013-07-18
/// \brief  Implementation for Transform class

#include "pbj/transform.h"
#include "pbj/scene/entity.h"

using pbj::scene::Entity;

namespace pbj {

///////////////////////////////////////////////////////////////////////////////
/// \brief Constructor for Transform class
/// \details Sets the position, rotation and scale for the Transform to
///          defaults.  For position this is the origin, rotation the
///          identity quaternion, and for scale 1.0.
Transform::Transform(void* owner) :
	_owner(owner)
{
	_position = vec2(0.0f, 0.0f);
	_rotation = 0.0f;
	_scale = vec2(1.0f, 1.0f);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Destructor for Transform class, left empty.
Transform::~Transform()
{}

///////////////////////////////////////////////////////////////////////////////
/// \brief Rotate the Transform along the z axis.
/// \param angle The angle to rotate, in degrees.
void Transform::rotate(F32 angle)
{
	while (_rotation >= 360)
		_rotation -= 360 + angle;

	while (_rotation < 0)
		_rotation += 360 + angle;

	/*
	if(_owner->rigidbody!=0)
	{
		//convert y rotation to euler angles and set the rigidbody accordingly
	}
	*/

}

///////////////////////////////////////////////////////////////////////////////
/// \brief Move the Transform.
/// \param dx The distance to move along the x-axis.
/// \param dy The distance to move along the y-axis.
/// \param dz The distance to move along the z-axis.
void Transform::move(F32 dx, F32 dy)
{
	move(vec2(dx, dy));	
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Move the transform
/// \param deltas A glm::vec3 containing the distance to move along each axis.
void Transform::move(const vec2& deltas)
{
	_position += deltas;
	/*
	if(_owner->rigidbody!=0)
		_owner->rigidbody->moveBody(_position.x, _position.y);
	*/
}

///////////////////////////////////////////////////////////////////////////////
/// \brief The position of the Transform.
/// \return The position of the Transform as a glm::vec3 (x,y,z).
const vec2& Transform::getPosition() const
{
	return _position;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Sets the position of the Transform
/// \param x The x position
/// \param y The y position
/// \param z The z position
void Transform::setPosition(F32 x, F32 y)
{
	_position = vec2(x, y);
	/*
	if(_owner->rigidbody!=0)
		_owner->rigidbody->moveBody(_position.x, _position.y);
	*/
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Sets the position of the Transform
/// \param pos The new position as a glm::vec3.
void Transform::setPosition(const vec2& pos)
{
	setPosition(pos.x, pos.y);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Get the rotation of the Transform as a quaternion
/// \return A quaternion representing the rotation of the Transform.
/// \details I have no idea why we might want to get the rotation is this manner
///          but it seems like a good idea.  So here it is.
F32 Transform::getRotation() const
{
	return _rotation;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Sets the rotation of the Transform
/// \param rotation The new rotation as a F32.
void Transform::setRotation(F32 rotation)
{
	_rotation = rotation;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Get the scale of the Transform.
/// \return A glm::vec3 representing the scale along the x-, y-, and z- axis.
const vec2& Transform::getScale() const
{
	return _scale;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Set the scale of the Transform
/// \param x The scale along the x-axis.
/// \param y The scale along the y-axis.
/// \param z The scale along the z-axis.
void Transform::setScale(F32 x, F32 y)
{
	_scale = glm::vec2(x, y);
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Set the scale of the Transform
/// \param scale A glm::vec3 of for the scale in the x, y, and z directions.
void Transform::setScale(const vec2& scale)
{
	_scale = scale;
}

///////////////////////////////////////////////////////////////////////////////
/// \brief Get the Transform as a 4x4 matrix.
/// \returns The Transform as a 4x4 matrix.
/// \details Since our call require a transformation matrix, this will provide
///          one.
//mat4 Transform::getMatrix() const
//{
//	mat4 ret = mat4();
//	vec3 v = _position.xyz;
//	ret = glm::translate(ret, v);
//	ret = glm::rotate(ret, glm::angle(_rotation), glm::axis(_rotation));
//	ret = glm::scale(ret, _scale);
//	return ret;
//}


void* Transform::getOwner()
{
	return _owner;
}

void Transform::updateOwnerRigidbody()
{
	
	Entity* e = (Entity*)_owner;
	if(e)
	{
		
	}
}

} // namespace pbj
