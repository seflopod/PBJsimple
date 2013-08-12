#ifndef PLAYER_COMPONENT_H_
#include "pbj/scene/player_component.h"
#endif

#include "pbj/scene/entity.h"

namespace pbj
{
namespace scene
{

PlayerComponent::PlayerComponent(PlayerStats stats, void* owner)
{
	_stats = stats;
	_owner = owner;
	_canJump = true;
	_thrusting = false;
	_forceFullRegen = false;
}

PlayerComponent::~PlayerComponent()
{
	_owner = 0;
}

PlayerStats PlayerComponent::getStats() const {	return _stats; }
void PlayerComponent::resetStats() { _stats = PlayerStats(); }

I32 PlayerComponent::getHealth() const { return _stats.health; }
I32 PlayerComponent::getMaxHealth() const { return _stats.maxHealth; }
I32 PlayerComponent::getFuelRemaining() const { return _stats.fuelRemaining; }
I32 PlayerComponent::getMaxFuel() const { return _stats.maxFuel; }
I32 PlayerComponent::getAmmoRemaining() const { return _stats.ammoRemaining; }
I32 PlayerComponent::getMaxAmmo() const { return _stats.maxAmmo; }
F32 PlayerComponent::getRateOfFire() const { return _stats.rateOfFire; }
F32 PlayerComponent::getJumpSpeed() const { return _stats.jumpSpeed; }
F32 PlayerComponent::getMoveSpeed() const { return _stats.moveSpeed; }
F32 PlayerComponent::getThrust() const { return _stats.thrust; }

void PlayerComponent::setHealth(I32 val) { _stats.health = val; }
void PlayerComponent::setMaxHealth(I32 val) { _stats.maxHealth = val; }

void PlayerComponent::setFuelRemaining(I32 val)
{
	if(val > _stats.maxFuel)
		val = _stats.maxFuel;
	if(val < 0)
		val = 0;
	_stats.fuelRemaining = val;
}

void PlayerComponent::setMaxFuel(I32 val) { _stats.maxFuel = val; }
void PlayerComponent::setAmmoRemaining(I32 val) { _stats.ammoRemaining = val; }
void PlayerComponent::setMaxAmmo(I32 val) { _stats.maxAmmo = val; }
void PlayerComponent::setRateOfFire(F32 val) { _stats.rateOfFire = val; }
void PlayerComponent::setJumpSpeed(F32 val) { _stats.jumpSpeed = val; }
void PlayerComponent::setMoveSpeed(F32 val) { _stats.moveSpeed = val; }
void PlayerComponent::setThrust(F32 val) { _stats.thrust = val; }

void* PlayerComponent::getOwner() const { return _owner; }

bool PlayerComponent::canJump() const { return _canJump; }
void PlayerComponent::disableJump() { _canJump = false; }
void PlayerComponent::enableJump() { _canJump = true; }

bool PlayerComponent::isThrusting() const { return _thrusting; }
void PlayerComponent::startThrust() { _thrusting = true; }
void PlayerComponent::endThrust() { _thrusting = false; }

void PlayerComponent::doThrust()
{
	I32 costPerTick = 100;
	Entity* const e = (Entity* const)_owner;
	physics::Rigidbody* const r = e->getRigidbody();
	if(r)
	{
		if(!_thrusting && !_forceFullRegen && _stats.fuelRemaining >= 2*costPerTick)
		{   //start thrusting
			vec2 force = vec2(0.0f, 3*_stats.thrust);
			r->applyForce(force, physics::Rigidbody::ForceMode::Impulse);
			_thrusting = true;
			_stats.fuelRemaining -= 2 * costPerTick;
			if(_stats.fuelRemaining < costPerTick/2)
				_forceFullRegen = true;
		}
		else if(_thrusting && !_forceFullRegen && _stats.fuelRemaining >= costPerTick)
		{
			_stats.fuelRemaining-=costPerTick;
			vec2 force = vec2(0.0f, _stats.thrust);
			r->applyForce(force, physics::Rigidbody::ForceMode::Constant);
			if(_stats.fuelRemaining < costPerTick/2)
				_forceFullRegen = true;
		}
		else
		{
			_thrusting = false;
		}
	}
}

void PlayerComponent::regenFuel()
{
	if(_stats.fuelRemaining < _stats.maxFuel && !_thrusting)
	{
		I32 regenPerTick = 25;
		_stats.fuelRemaining += regenPerTick;
		if(_stats.fuelRemaining >= _stats.maxFuel)
		{
			_stats.fuelRemaining = _stats.maxFuel;
			_forceFullRegen = false;
		}
	}
}

} //namespace scene
} //namespace pbj