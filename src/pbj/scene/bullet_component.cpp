#ifndef BULLET_COMPONENT_H_
#include "pbj/scene/bullet_component.h"
#endif

#include <assert.h>
#include "pbj/scene/entity.h"

namespace pbj
{
namespace scene
{

BulletComponent::BulletComponent(void* owner) :
					_shooter(nullptr)
{
	assert((Entity*)owner);
	_owner = owner;
}

BulletComponent::~BulletComponent()
{
	_owner = nullptr;
	_shooter = nullptr;
}

void BulletComponent::setShooter(void* shooter)
{
	assert((Entity*)shooter);
	_shooter = shooter;
}

void* BulletComponent::getShooter() { return _shooter; }

void* BulletComponent::getOwner() const
{
    return _owner;
}

}
}