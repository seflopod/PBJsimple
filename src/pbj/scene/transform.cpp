///////////////////////////////////////////////////////////////////////////////
/// \file   pbj/transform.cpp
/// \author Peter Bartosch / Josh Douglas
/// \date   2013-07-18
/// \brief  Implementation for Transform class

#include "pbj/scene/transform.h"
#include "pbj/scene/entity.h"

using pbj::scene::Entity;

namespace pbj {
namespace scene {

////////////////////////////////////////////////////////////////////////////////
/// \fn Transform::Transform(Entity* owner)
///
/// \brief  Constructor for Transform class.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param [in,out] owner   If non-null, the owner.
///
/// \details Sets the position, rotation and scale for the Transform to
///          defaults.  For position this is the origin, rotation the
///          identity quaternion, and for scale 1.0.
////////////////////////////////////////////////////////////////////////////////
Transform::Transform(Entity* owner)
	: _owner(owner)
{
	_position = vec2(0.0f, 0.0f);
	_rotation = 0.0;
	_scale = vec2(1.0f);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn Transform::~Transform()
///
/// \brief  Destructor for Transform class, left empty.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
Transform::~Transform()
{}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Transform::rotate(F32 angle)
///
/// \brief  Rotate the Transform along the z axis.
///
/// \author Peter Bartosch / Josh Douglas
/// \date   2013-08-22
///
/// \param  angle   The angle to rotate, in degrees.
////////////////////////////////////////////////////////////////////////////////
void Transform::rotate(F32 angle)
{
	while (_rotation >= 360)
	{
		_rotation -= 360 + angle;
	}

	while (_rotation < 0)
	{
		_rotation += 360 + angle;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Transform::move(F32 dx, F32 dy)
///
/// \brief  Move the Transform.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  dx  The distance to move along the x-axis.
/// \param  dy  The distance to move along the y-axis.
////////////////////////////////////////////////////////////////////////////////
void Transform::move(F32 dx, F32 dy) { move(vec2(dx, dy)); }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Transform::move(const vec2& deltas)
///
/// \brief  Move the transform.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  deltas  A glm::vec3 containing the distance to move along each
///                 axis.
////////////////////////////////////////////////////////////////////////////////
void Transform::move(const vec2& deltas) { _position += deltas; }

////////////////////////////////////////////////////////////////////////////////
/// \fn const vec2& Transform::getPosition() const
///
/// \brief  The position of the Transform.
///
/// \author Peter Bartosch / Josh Douglas
/// \date   2013-08-22
///
/// \return The position of the Transform as a glm::vec2 (x,y).
////////////////////////////////////////////////////////////////////////////////
const vec2& Transform::getPosition() const { return _position; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Transform::setPosition(F32 x, F32 y)
///
/// \brief  Sets the position of the Transform.
///
/// \author Peter Bartosch / Josh Douglas
/// \date   2013-08-22
///
/// \param  x   The x position.
/// \param  y   The y position.
////////////////////////////////////////////////////////////////////////////////
void Transform::setPosition(F32 x, F32 y) { _position = vec2(x, y); }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Transform::setPosition(const vec2& pos)
///
/// \brief  Sets the position of the Transform.
///
/// \author Peter Bartosch / Josh Douglas
/// \date   2013-08-22
///
/// \param  pos The new position as a glm::vec2.
////////////////////////////////////////////////////////////////////////////////
void Transform::setPosition(const vec2& pos) { setPosition(pos.x, pos.y); }

////////////////////////////////////////////////////////////////////////////////
/// \fn F32 Transform::getRotation() const
///
/// \brief  Get the rotation of the Transform.
///
/// \author Peter Bartosch / Josh Douglas
/// \date   2013-08-22
///
/// \return A float representing the rotation of the Transform.
////////////////////////////////////////////////////////////////////////////////
F32 Transform::getRotation() const { return _rotation; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Transform::setRotation(F32 rotation)
///
/// \brief  Sets the rotation of the Transform.
///
/// \author Peter Bartosch / Josh Douglas
/// \date   2013-08-22
///
/// \param  rotation    The new rotation as a F32.
////////////////////////////////////////////////////////////////////////////////
void Transform::setRotation(F32 rotation) { _rotation = rotation; }

////////////////////////////////////////////////////////////////////////////////
/// \fn const vec2& Transform::getScale() const
///
/// \brief  Get the scale of the Transform.
///
/// \author Peter Bartosch / Josh Douglas
/// \date   2013-08-22
///
/// \return A glm::vec2 representing the scale along the x-axis and y-axis.
////////////////////////////////////////////////////////////////////////////////
const vec2& Transform::getScale() const { return _scale; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Transform::setScale(F32 x, F32 y)
///
/// \brief  Set the scale of the Transform.
///
/// \author Peter Bartosch / Josh Douglas
/// \date   2013-08-22
///
/// \param  x   The scale along the x-axis.
/// \param  y   The scale along the y-axis.
////////////////////////////////////////////////////////////////////////////////
void Transform::setScale(F32 x, F32 y) { _scale = glm::vec2(x, y); }

////////////////////////////////////////////////////////////////////////////////
/// \fn void Transform::setScale(const vec2& scale)
///
/// \brief  Set the scale of the Transform.
///
/// \author Peter Bartosch / Josh Douglas
/// \date   2013-08-22
///
/// \param  scale   A glm::vec2 of for the scale in the x and y directions.
////////////////////////////////////////////////////////////////////////////////
void Transform::setScale(const vec2& scale) { _scale = scale; }

////////////////////////////////////////////////////////////////////////////////
/// \fn Entity* Transform::getOwner()
///
/// \brief  Gets the owner of this item.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the owner.
////////////////////////////////////////////////////////////////////////////////
Entity* Transform::getOwner()
{
	return _owner;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void Transform::updateOwnerRigidbody()
///
/// \brief  Updates the owner rigidbody.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///  
/// \details If an Entity moves its Transform it needs a way of moving the
///          rigidbody as well, so that's what this is for.  There aren't too
///          many uses for this since motion is handled by the Rigidbody, but
///          for spawning purposes this is needed.
////////////////////////////////////////////////////////////////////////////////
void Transform::updateOwnerRigidbody()
{
	Entity* e = 0;
    e = (Entity*)_owner;
	if(e && e->getRigidbody())
	    e->getRigidbody()->setTransform(toB2(_position), toB2(_scale), _rotation);
    e = 0;
}

} // namespace pbj::scene
} // namespace pbj
