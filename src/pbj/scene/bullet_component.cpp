////////////////////////////////////////////////////////////////////////////////
/// \file   Z:\Documents\PBJsimple\src\pbj\scene\bullet_component.cpp
///
/// \brief  Implements the bullet component class.
////////////////////////////////////////////////////////////////////////////////

#include "pbj/scene/bullet_component.h"

#include "pbj/scene/entity.h"
#include <cassert>

namespace pbj
{
namespace scene
{
////////////////////////////////////////////////////////////////////////////////
/// \fn BulletComponent::BulletComponent(void* owner)
///
/// \brief  Constructor.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param [in,out] owner   If non-null, the owner.
////////////////////////////////////////////////////////////////////////////////
BulletComponent::BulletComponent(void* owner) :
                    _shooter(nullptr)
{
    assert((Entity*)owner);
    _owner = owner;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn BulletComponent::~BulletComponent()
///
/// \brief  Destructor.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
BulletComponent::~BulletComponent()
{
    _owner = nullptr;
    _shooter = nullptr;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void BulletComponent::setShooter(void* shooter)
///
/// \brief  Sets the shooter.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param [in] shooter The Entity that shot the bullet.
////////////////////////////////////////////////////////////////////////////////
void BulletComponent::setShooter(void* shooter)
{
    assert((Entity*)shooter);
    _shooter = shooter;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void* BulletComponent::getShooter()
///
/// \brief  Gets the shooter of this BulletComponent.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the shooter.
////////////////////////////////////////////////////////////////////////////////
void* BulletComponent::getShooter() { return _shooter; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void* BulletComponent::getOwner() const
///
/// \brief  Gets the owner of this item.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return null if it fails, else the owner.
////////////////////////////////////////////////////////////////////////////////
void* BulletComponent::getOwner() const { return _owner; }

} //namespace scene
} //namespace pbj