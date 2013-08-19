////////////////////////////////////////////////////////////////////////////////
/// \file	C:\Users\pbartosch_sa\Documents\Visual Studio 2012\Projects\
/// 		PBJsimple\src\pbj\scene\player_component.cpp
///
/// \brief	Implements the player component class.
#ifndef PLAYER_COMPONENT_H_
#include "pbj/scene/player_component.h"
#endif

#include "pbj/scene/entity.h"
#include "pbj/game.h"

namespace pbj
{
namespace scene
{
////////////////////////////////////////////////////////////////////////////////
/// \brief	Constructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	stats		 	The stats.
/// \param [in,out]	owner	If non-null, the owner.
PlayerComponent::PlayerComponent(PlayerStats stats, Entity* owner)
    : _stats(stats),
      _owner(owner),
      _canJump(true),
      _thrusting(false),
      _forceFullRegen(false),
      _canShoot(true),
      _reloading(false),
      _fireCooldown(false)
{
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	Destructor.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
PlayerComponent::~PlayerComponent()
{
	_owner = 0;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	Gets the stats.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	The stats.
PlayerStats PlayerComponent::getStats() const
{
    return _stats;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	Resets the stats.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
void PlayerComponent::resetStats()
{
    _stats = PlayerStats();
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	Gets the health.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	The health.
I32 PlayerComponent::getHealth() const
{
    return _stats.health;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	Gets maximum health.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	The maximum health.
I32 PlayerComponent::getMaxHealth() const
{
    return _stats.maxHealth;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	Gets fuel remaining.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	The fuel remaining.
////////////////////////////////////////////////////////////////////////////////
I32 PlayerComponent::getFuelRemaining() const
{
    return _stats.fuelRemaining;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	Gets maximum fuel.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	The maximum fuel.
////////////////////////////////////////////////////////////////////////////////
I32 PlayerComponent::getMaxFuel() const
{
    return _stats.maxFuel;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	I32 PlayerComponent::getAmmoRemaining() const
///
/// \brief	Gets ammo remaining.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	The ammo remaining.
I32 PlayerComponent::getAmmoRemaining() const
{
    return _stats.ammoRemaining;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	I32 PlayerComponent::getMaxAmmo() const
///
/// \brief	Gets maximum ammo.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	The maximum ammo.
I32 PlayerComponent::getMaxAmmo() const
{
    return _stats.maxAmmo;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	F32 PlayerComponent::getRateOfFire() const
///
/// \brief	Gets rate of fire.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	The rate of fire.
F32 PlayerComponent::getRateOfFire() const
{
    return _stats.rateOfFire;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	F32 PlayerComponent::getJumpSpeed() const
///
/// \brief	Gets jump speed.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	The jump speed.
F32 PlayerComponent::getJumpSpeed() const
{
    return _stats.jumpSpeed;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	F32 PlayerComponent::getMoveSpeed() const
///
/// \brief	Gets move speed.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	The move speed.
F32 PlayerComponent::getMoveSpeed() const
{
    return _stats.moveSpeed;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	F32 PlayerComponent::getThrust() const
///
/// \brief	Gets the thrust.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	The thrust.
F32 PlayerComponent::getThrust() const
{
    return _stats.thrust;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::setHealth(I32 val)
///
/// \brief	Sets a health.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	val	The value.
void PlayerComponent::setHealth(I32 val)
{
    _stats.health = val;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::setMaxHealth(I32 val)
///
/// \brief	Sets maximum health.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	val	The value.
void PlayerComponent::setMaxHealth(I32 val)
{
    _stats.maxHealth = val;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::setFuelRemaining(I32 val)
///
/// \brief	Sets fuel remaining.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	val	The value.
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::setFuelRemaining(I32 val)
{
	if(val > _stats.maxFuel)
		val = _stats.maxFuel;
	if(val < 0)
		val = 0;
	_stats.fuelRemaining = val;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::setMaxFuel(I32 val)
///
/// \brief	Sets maximum fuel.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	val	The value.
void PlayerComponent::setMaxFuel(I32 val)
{
    _stats.maxFuel = val;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::setAmmoRemaining(I32 val)
///
/// \brief	Sets ammo remaining.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	val	The value.
void PlayerComponent::setAmmoRemaining(I32 val)
{
    _stats.ammoRemaining = val;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::setMaxAmmo(I32 val)
///
/// \brief	Sets maximum ammo.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	val	The value.
void PlayerComponent::setMaxAmmo(I32 val)
{
    _stats.maxAmmo = val;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::setRateOfFire(F32 val)
///
/// \brief	Sets rate of fire.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	val	The value.
void PlayerComponent::setRateOfFire(F32 val)
{
    _stats.rateOfFire = val;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::setJumpSpeed(F32 val)
///
/// \brief	Sets jump speed.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	val	The value.
void PlayerComponent::setJumpSpeed(F32 val)
{
    _stats.jumpSpeed = val;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::setMoveSpeed(F32 val)
///
/// \brief	Sets move speed.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	val	The value.
void PlayerComponent::setMoveSpeed(F32 val) {
    _stats.moveSpeed = val;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::setThrust(F32 val)
///
/// \brief	Sets a thrust.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	val	The value.
void PlayerComponent::setThrust(F32 val) {
    _stats.thrust = val;
}

////////////////////////////////////////////////////////////////////////////////
/// \brief	Gets the owner of this item.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	null if it fails, else the owner.
Entity* PlayerComponent::getOwner() const
{
    return _owner;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool PlayerComponent::canJump() const
///
/// \brief	Determine if we can jump.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	true if we can jump, false if not.
bool PlayerComponent::canJump() const
{
    return _canJump;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::disableJump()
///
/// \brief	Disables the jump.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
void PlayerComponent::disableJump()
{
    _canJump = false;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::enableJump()
///
/// \brief	Enables the jump.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
void PlayerComponent::enableJump()
{
    _canJump = true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool PlayerComponent::isThrusting() const
///
/// \brief	Query if this object is thrusting.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	true if thrusting, false if not.
bool PlayerComponent::isThrusting() const
{
    return _thrusting;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::startThrust()
///
/// \brief	Starts a thrust.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
void PlayerComponent::startThrust()
{
    _thrusting = true;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::endThrust()
///
/// \brief	Ends a thrust.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
void PlayerComponent::endThrust()
{
    _thrusting = false;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::doThrust()
///
/// \brief	Executes the thrust operation.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
void PlayerComponent::doThrust()
{
	I32 costPerTick = 100;
	physics::Rigidbody* const r = _owner->getRigidbody();
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::regenFuel()
///
/// \brief	Regen fuel.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
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

////////////////////////////////////////////////////////////////////////////////
/// \fn	F32 PlayerComponent::getReloadTimer() const
///
/// \brief	Gets reload timer.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	The reload timer.
F32 PlayerComponent::getReloadTimer() const
{
	return _reloadTimer;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	F32 PlayerComponent::getFireTimer() const
///
/// \brief	Gets fire timer.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	The fire timer.
F32 PlayerComponent::getFireTimer() const
{
	return _fireTimer;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool PlayerComponent::fireOnCooldown() const
///
/// \brief	Determines if we can fire on cooldown.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	true if it succeeds, false if it fails.
bool PlayerComponent::fireOnCooldown() const
{
	return _fireCooldown;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool PlayerComponent::canShoot() const
///
/// \brief	Determine if we can shoot.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	true if we can shoot, false if not.
bool PlayerComponent::canShoot() const
{
	return _canShoot;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	bool PlayerComponent::reloading() const
///
/// \brief	Reloadings this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \return	true if it succeeds, false if it fails.
bool PlayerComponent::reloading() const
{
	return _reloading;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::stepReloadTimer(F32 dt)
///
/// \brief	Step reload timer.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	dt	The dt.
void PlayerComponent::stepReloadTimer(F32 dt)
{
	_reloadTimer+=dt;
	_reloading = _reloadTimer < _stats.reloadTime;
	_canShoot = !_reloading;
	if(_canShoot)
	{
		_stats.ammoRemaining = _stats.maxAmmo;
		std::cerr<<"Done reloading"<<std::endl;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::resetReloadTimer()
///
/// \brief	Resets the reload timer.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
void PlayerComponent::resetReloadTimer()
{
	_reloadTimer = 0.0f;
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::stepFireTimer(F32 dt)
///
/// \brief	Step fire timer.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	dt	The dt.
void PlayerComponent::stepFireTimer(F32 dt)
{
	_fireTimer+=dt;
	if(_fireTimer >= _stats.rateOfFire)
	{
		_fireCooldown = false;
		_canShoot = true;
		_fireTimer = 0.0f;
	}

}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::fire(F32 mouseX, F32 mouseY)
///
/// \brief	Fires.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
///
/// \param	mouseX	The mouse x coordinate.
/// \param	mouseY	The mouse y coordinate.
void PlayerComponent::fire(F32 mouseX, F32 mouseY)
{
	if(_canShoot && !_fireCooldown && _stats.health > 0)
	{
        Transform& xf = _owner->getTransform();

		F32 bulletSpeed = 50.0f;
		vec2 pos = xf.getPosition();

		//the 0.5f used here is supposed to take the size of the bullet into
		//account.  Obviously if the size of the bullet changes than this needs
		//to change.
		if(mouseX < pos.x)
		{
			pos.x -= (xf.getScale().x/2 + 0.75f);
		}
		else if(mouseX > pos.x)
		{
			pos.x += (xf.getScale().x/2 + 0.75f);
		}
		else if(mouseY < pos.y)
		{
			pos.y -= (xf.getScale().y/2 + 0.75f);
		}
		else if(mouseY > pos.y)
		{
			pos.y += (xf.getScale().y/2 + 0.75f);
		}
		else
		{   //if we're here, we're clicking on the player
			return;
		}
		vec2 diff = vec2(mouseX,mouseY) - pos;
		F32 ang = std::atan2(diff.y,diff.x);
		vec2 velDir = vec2(std::cos(ang), std::sin(ang));
		Game::instance()->spawnBullet(pos, velDir * bulletSpeed);
		_stats.ammoRemaining -= 1;
		if(_stats.ammoRemaining <= 0)
		{
			std::cerr<<"Reloading"<<std::endl;
			_stats.ammoRemaining = 0;
			_reloadTimer = 0.0f;
			_reloading = true;
		}
		else
		{
			_fireTimer = 0.0f;
			_fireCooldown = true;
		}
		_canShoot = false;
	}
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::moveLeft()
///
/// \brief	Move left.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
void PlayerComponent::moveLeft()
{
	Entity* e = (Entity*)_owner;
	vec2 vel = e->getRigidbody()->getVelocity();
	vel.x = -1 * _stats.moveSpeed;
	e->getRigidbody()->setVelocity(vel);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::moveRight()
///
/// \brief	Move right.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
void PlayerComponent::moveRight()
{
	Entity* e = (Entity*)_owner;
	vec2 vel = e->getRigidbody()->getVelocity();
	vel.x = _stats.moveSpeed;
	e->getRigidbody()->setVelocity(vel);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn	void PlayerComponent::jump()
///
/// \brief	Jumps this object.
///
/// \author	Peter Bartosch
/// \date	2013-08-13
void PlayerComponent::jump()
{
	if(_canJump)
	{
		Entity* e = (Entity*)_owner;
		vec2 vel = e->getRigidbody()->getVelocity();
		vel.y = _stats.jumpSpeed;
		e->getRigidbody()->setVelocity(vel);
		_canJump = false;
	}
}

////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::takeDamage(I32 dmg)
{
	_stats.health-=dmg;
	std::cerr<<_stats.health<<std::endl;
	if(_stats.health <= 0)
	{
		std::cerr<<"Dead"<<std::endl;
	}
	std::cerr<<std::endl;
}

////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::stop()
{
	Entity* e = (Entity*)_owner;
	vec2 vel = e->getRigidbody()->getVelocity();
	vel.x *= 0.4f;
	vel.y = (vel.y>0.0f) ? vel.y*0.8f : vel.y*1.2f;
	e->getRigidbody()->setVelocity(vel);
}

////////////////////////////////////////////////////////////////////////////////
bool PlayerComponent::isDead()
{
    return _stats.health <= 0;
}

////////////////////////////////////////////////////////////////////////////////
F64 PlayerComponent::getTimeOfDeath()
{
    return _timeOfDeath;
}

////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::setTimeOfDeath(F64 tod)
{
    _timeOfDeath = tod;
}

} //namespace scene
} //namespace pbj
