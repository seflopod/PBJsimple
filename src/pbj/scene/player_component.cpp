////////////////////////////////////////////////////////////////////////////////
/// \file   Z:\Documents\PBJsimple\src\pbj\scene\player_component.cpp
///
/// \brief  Implements the player component class.
////////////////////////////////////////////////////////////////////////////////
#ifndef PLAYER_COMPONENT_H_
#include "pbj/scene/player_component.h"
#endif

#include <assert.h>
#include "pbj/scene/entity.h"
#include "pbj/game.h"

namespace pbj
{
namespace scene
{
////////////////////////////////////////////////////////////////////////////////
/// \brief    Constructor.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    stats             The stats.
/// \param [in]    owner    If non-null, the owner.
////////////////////////////////////////////////////////////////////////////////
PlayerComponent::PlayerComponent(Id id, PlayerStats stats, void* owner)
    : _id(id),
        _stats(stats),
        _canJump(true),
        _thrusting(false),
        _forceFullRegen(false),
        _canShoot(true),
        _reloading(false),
        _fireCooldown(false)
{
    assert((Entity*)owner);
    _owner = owner;
    _score = PlayerScore();
}

////////////////////////////////////////////////////////////////////////////////
/// \brief    Destructor.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
////////////////////////////////////////////////////////////////////////////////
PlayerComponent::~PlayerComponent()
{
    _owner = 0;
}

Id PlayerComponent::getId() const { return _id; }

#pragma region stats
////////////////////////////////////////////////////////////////////////////////
/// \brief    Gets the stats.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    The stats.
////////////////////////////////////////////////////////////////////////////////
PlayerStats PlayerComponent::getStats() const { return _stats; }

////////////////////////////////////////////////////////////////////////////////
/// \brief    Resets the stats.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::resetStats() { _stats = PlayerStats(); }

////////////////////////////////////////////////////////////////////////////////
/// \brief    Gets the health.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    The health.
////////////////////////////////////////////////////////////////////////////////
I32 PlayerComponent::getHealth() const { return _stats.health; }

////////////////////////////////////////////////////////////////////////////////
/// \fn bool PlayerComponent::isDead()
///
/// \brief  Query if this PlayerComponent is dead.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return true if dead, false if not.
////////////////////////////////////////////////////////////////////////////////
bool PlayerComponent::isDead() { return _stats.health <= 0; }

////////////////////////////////////////////////////////////////////////////////
/// \brief    Gets maximum health.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    The maximum health.
////////////////////////////////////////////////////////////////////////////////
I32 PlayerComponent::getMaxHealth() const { return _stats.maxHealth; }

////////////////////////////////////////////////////////////////////////////////
/// \brief    Gets fuel remaining.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    The fuel remaining.
////////////////////////////////////////////////////////////////////////////////
I32 PlayerComponent::getFuelRemaining() const { return _stats.fuelRemaining; }

////////////////////////////////////////////////////////////////////////////////
/// \brief    Gets maximum fuel.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    The maximum fuel.
////////////////////////////////////////////////////////////////////////////////
I32 PlayerComponent::getMaxFuel() const { return _stats.maxFuel; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    I32 PlayerComponent::getAmmoRemaining() const
///
/// \brief    Gets ammo remaining.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    The ammo remaining.
I32 PlayerComponent::getAmmoRemaining() const { return _stats.ammoRemaining; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    I32 PlayerComponent::getMaxAmmo() const
///
/// \brief    Gets maximum ammo.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    The maximum ammo.
I32 PlayerComponent::getMaxAmmo() const { return _stats.maxAmmo; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    F32 PlayerComponent::getRateOfFire() const
///
/// \brief    Gets rate of fire.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    The rate of fire.
F32 PlayerComponent::getRateOfFire() const { return _stats.rateOfFire; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    F32 PlayerComponent::getJumpSpeed() const
///
/// \brief    Gets jump speed.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    The jump speed.
F32 PlayerComponent::getJumpSpeed() const { return _stats.jumpSpeed; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    F32 PlayerComponent::getMoveSpeed() const
///
/// \brief    Gets move speed.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    The move speed.
F32 PlayerComponent::getMoveSpeed() const { return _stats.moveSpeed; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    F32 PlayerComponent::getThrust() const
///
/// \brief    Gets the thrust.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    The thrust.
F32 PlayerComponent::getThrust() const { return _stats.thrust; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    void PlayerComponent::setHealth(I32 val)
///
/// \brief    Sets a health.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    val    The value.
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::setHealth(I32 val) { _stats.health = val; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    void PlayerComponent::setMaxHealth(I32 val)
///
/// \brief    Sets maximum health.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    val    The value.
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::setMaxHealth(I32 val) { _stats.maxHealth = val; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    void PlayerComponent::setFuelRemaining(I32 val)
///
/// \brief    Sets fuel remaining.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    val    The value.
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
/// \fn    void PlayerComponent::setMaxFuel(I32 val)
///
/// \brief    Sets maximum fuel.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    val    The value.
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::setMaxFuel(I32 val) { _stats.maxFuel = val; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    void PlayerComponent::setAmmoRemaining(I32 val)
///
/// \brief    Sets ammo remaining.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    val    The value.
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::setAmmoRemaining(I32 val) { _stats.ammoRemaining = val; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    void PlayerComponent::setMaxAmmo(I32 val)
///
/// \brief    Sets maximum ammo.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    val    The value.
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::setMaxAmmo(I32 val) { _stats.maxAmmo = val; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    void PlayerComponent::setRateOfFire(F32 val)
///
/// \brief    Sets rate of fire.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    val    The value.
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::setRateOfFire(F32 val) { _stats.rateOfFire = val; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    void PlayerComponent::setJumpSpeed(F32 val)
///
/// \brief    Sets jump speed.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    val    The value.
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::setJumpSpeed(F32 val) { _stats.jumpSpeed = val; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    void PlayerComponent::setMoveSpeed(F32 val)
///
/// \brief    Sets move speed.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    val    The value.
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::setMoveSpeed(F32 val) { _stats.moveSpeed = val; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    void PlayerComponent::setThrust(F32 val)
///
/// \brief    Sets a thrust.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \param    val    The value.
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::setThrust(F32 val) { _stats.thrust = val; }

#pragma endregion
////////////////////////////////////////////////////////////////////////////////
/// \brief    Gets the owner of this item.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    null if it fails, else the owner.
////////////////////////////////////////////////////////////////////////////////
void* PlayerComponent::getOwner() const { return _owner; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    bool PlayerComponent::canJump() const
///
/// \brief    Determine if we can jump.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    true if we can jump, false if not.
////////////////////////////////////////////////////////////////////////////////
bool PlayerComponent::canJump() const { return _canJump; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    void PlayerComponent::disableJump()
///
/// \brief    Disables the ability to jump.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::disableJump() { _canJump = false; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    void PlayerComponent::enableJump()
///
/// \brief    Enables the jump.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::enableJump() { _canJump = true; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    bool PlayerComponent::isThrusting() const
///
/// \brief    Query if this object is thrusting.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    true if thrusting, false if not.
////////////////////////////////////////////////////////////////////////////////
bool PlayerComponent::isThrusting() const { return _thrusting; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    void PlayerComponent::startThrust()
///
/// \brief    Starts a thrust.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::startThrust() { _thrusting = true; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    void PlayerComponent::endThrust()
///
/// \brief    Ends a thrust.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::endThrust() { _thrusting = false; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    void PlayerComponent::doThrust()
///
/// \brief    Executes the thrust operation.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::doThrust()
{
    I32 costPerTick = 100;
    physics::Rigidbody* const r = ((Entity*)_owner)->getRigidbody();
    if(r) //if there's no Rigidbody, there's no point.
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
/// \fn    void PlayerComponent::regenFuel()
///
/// \brief    Regen fuel.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::regenFuel()
{
    if(_stats.fuelRemaining < _stats.maxFuel && !_thrusting)
    {
        I32 regenPerTick = 25; //magic!
        _stats.fuelRemaining += regenPerTick;
        if(_stats.fuelRemaining >= _stats.maxFuel)
        {
            _stats.fuelRemaining = _stats.maxFuel;
            _forceFullRegen = false;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \fn    F32 PlayerComponent::getReloadTimer() const
///
/// \brief    Gets the amount of time since the reload started.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    The reload timer.
////////////////////////////////////////////////////////////////////////////////
F32 PlayerComponent::getReloadTimer() const { return _reloadTimer; }

////////////////////////////////////////////////////////////////////////////////
/// \fn    F32 PlayerComponent::getFireTimer() const
///
/// \brief    Gets amount of time since the last shot was fired.
///
/// \author    Peter Bartosch
/// \date    2013-08-13
///
/// \return    The fire timer.
////////////////////////////////////////////////////////////////////////////////
F32 PlayerComponent::getFireTimer() const { return _fireTimer; }

////////////////////////////////////////////////////////////////////////////////
/// \fn bool PlayerComponent::fireOnCooldown() const
///
/// \brief  Checks to see if the player can fire or if the ability to do so
///         is on cooldown.
///
/// \author Peter Bartosch
/// \date   2013-08-13
///
/// \return true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
bool PlayerComponent::fireOnCooldown() const { return _fireCooldown; }

////////////////////////////////////////////////////////////////////////////////
/// \fn bool PlayerComponent::canShoot() const
///
/// \brief  Determine if we can shoot.
///
/// \author Peter Bartosch
/// \date   2013-08-13
///
/// \return true if we can shoot, false if not.
////////////////////////////////////////////////////////////////////////////////
bool PlayerComponent::canShoot() const { return _canShoot; }

////////////////////////////////////////////////////////////////////////////////
/// \fn bool PlayerComponent::reloading() const
///
/// \brief  Sees if the player is reloading.
///
/// \author Peter Bartosch
/// \date   2013-08-13
///
/// \return true if it succeeds, false if it fails.
////////////////////////////////////////////////////////////////////////////////
bool PlayerComponent::reloading() const { return _reloading; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void PlayerComponent::stepReloadTimer(F32 dt)
///
/// \brief  Advance the reload timer by a step.
///
/// \author Peter Bartosch
/// \date   2013-08-13
///
/// \param  dt  The amount of time to advance the timer.
///
/// \details If the timer exceeds the reload time in the stats then the player
///          is given the ability to shoot and the maximum amount of ammo
///          possible.
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::stepReloadTimer(F32 dt)
{
    _reloadTimer+=dt;
    _reloading = _reloadTimer < _stats.reloadTime;
    _canShoot = !_reloading;
    if(_canShoot)
    {
        _stats.ammoRemaining = _stats.maxAmmo;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void PlayerComponent::resetReloadTimer()
///
/// \brief  Resets the reload timer.
///
/// \author Peter Bartosch
/// \date   2013-08-13
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::resetReloadTimer() { _reloadTimer = 0.0f; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void PlayerComponent::stepFireTimer(F32 dt)
///
/// \brief  Moves the fire timer forward a step.
///
/// \author Peter Bartosch
/// \date   2013-08-13
///
/// \param  dt    The amount of time to move the timer.
///
/// \details    If the timer exceeds the amount specified in PlayerStats then
///             the player is given the ability to shoot.
////////////////////////////////////////////////////////////////////////////////
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
/// \fn void PlayerComponent::fire(F32 mouseX, F32 mouseY)
///
/// \brief  Fires the gun.
///
/// \author Peter Bartosch
/// \date   2013-08-13
///
/// \param  mouseX  The mouse x coordinate.
/// \param  mouseY  The mouse y coordinate.
///
/// \details    This will check to see if the player is out of ammo and set
///             the reload accordingly.  Also plays the sound for firing
///             (assuming it exists).
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::fire(F32 mouseX, F32 mouseY)
{
    if(_canShoot && !_fireCooldown && _stats.health > 0)
    {
        Transform& xf = ((Entity*)_owner)->getTransform();

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
        Game::instance()->spawnBullet(pos, velDir * bulletSpeed, _owner);
        _stats.ammoRemaining -= 1;
        if(_stats.ammoRemaining <= 0)
        {
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

        //play audio
        if(((Entity*)_owner)->getAudioSource())
        {
            ((Entity*)_owner)->getAudioSource()->stop();
            ((Entity*)_owner)->getAudioSource()->play("fire");
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void PlayerComponent::moveLeft()
///
/// \brief  Move left.
///
/// \author Peter Bartosch
/// \date   2013-08-13
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::moveLeft()
{
    Entity* e = (Entity*)_owner;
    vec2 vel = e->getRigidbody()->getVelocity();
    vel.x = -1 * _stats.moveSpeed;
    e->getRigidbody()->setVelocity(vel);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void PlayerComponent::moveRight()
///
/// \brief  Move right.
///
/// \author Peter Bartosch
/// \date   2013-08-13
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::moveRight()
{
    Entity* e = (Entity*)_owner;
    vec2 vel = e->getRigidbody()->getVelocity();
    vel.x = _stats.moveSpeed;
    e->getRigidbody()->setVelocity(vel);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn void PlayerComponent::jump()
///
/// \brief  Jumps this object.
///
/// \author Peter Bartosch
/// \date   2013-08-13
////////////////////////////////////////////////////////////////////////////////
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
/// \fn void PlayerComponent::takeDamage(I32 dmg)
///
/// \brief  Take damage and play relevant audio.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  dmg The damge.
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::takeDamage(I32 dmg)
{
    Entity* e = (Entity*)_owner;

    _stats.health-=dmg;

    if(e->getAudioSource())
    {
        e->getAudioSource()->stop();
        e->getAudioSource()->play("dmg");
    }
    if(_stats.health <= 0)
    {
        if(e->getAudioSource())
        {
            e->getAudioSource()->stop();
            e->getAudioSource()->play("death");
        }
    }

}

////////////////////////////////////////////////////////////////////////////////
/// \fn void PlayerComponent::stop()
///
/// \brief  Slows this PlayerComponent until it stops.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::stop()
{
    Entity* e = (Entity*)_owner;
    vec2 vel = e->getRigidbody()->getVelocity();
    vel.x *= 0.4f;

    //if the player is moving up the reduction is less than if they are moving
    //down
    vel.y = (vel.y>0.0f) ? vel.y*0.8f : vel.y*1.2f;
    e->getRigidbody()->setVelocity(vel);
}

////////////////////////////////////////////////////////////////////////////////
/// \fn F64 PlayerComponent::getTimeOfDeath()
///
/// \brief  Gets time of death.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return The time of death.
////////////////////////////////////////////////////////////////////////////////
F64 PlayerComponent::getTimeOfDeath() { return _timeOfDeath; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void PlayerComponent::setTimeOfDeath(F64 tod)
///
/// \brief  Sets time of death.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  tod The TOD.
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::setTimeOfDeath(F64 tod) { _timeOfDeath = tod; }

#pragma region scores
////////////////////////////////////////////////////////////////////////////////
/// \fn I32 PlayerComponent::getKills() const
///
/// \brief  Gets the number of kills this PlayerComponent has recorded.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return The kills.
////////////////////////////////////////////////////////////////////////////////
I32 PlayerComponent::getKills() const { return _score.kills; }

////////////////////////////////////////////////////////////////////////////////
/// \fn I32 PlayerComponent::getDeaths() const
///
/// \brief  Gets the number of times this PlayerComponent has died.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return The deaths.
////////////////////////////////////////////////////////////////////////////////
I32 PlayerComponent::getDeaths() const { return _score.deaths; }

////////////////////////////////////////////////////////////////////////////////
/// \fn I32 PlayerComponent::getBulletsFired() const
///
/// \brief  Gets the number of bullets fired.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return The bullets fired.
////////////////////////////////////////////////////////////////////////////////
I32 PlayerComponent::getBulletsFired() const { return _score.bulletsFired; }

////////////////////////////////////////////////////////////////////////////////
/// \fn I32 PlayerComponent::getBulletsHit() const
///
/// \brief  Gets the number of bullets that hit another PlayerComponent.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \return The bullets hit.
////////////////////////////////////////////////////////////////////////////////
I32 PlayerComponent::getBulletsHit() const { return _score.bulletsHit; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void PlayerComponent::setKills(I32 val)
///
/// \brief  Sets the number of kills.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  val The value.
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::setKills(I32 val) { _score.kills = val; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void PlayerComponent::setDeaths(I32 val)
///
/// \brief  Sets the number of deaths.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  val The value.
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::setDeaths(I32 val) { _score.deaths = val; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void PlayerComponent::setBulletsFired(I32 val)
///
/// \brief  Sets the number of bullets fired.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  val The value.
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::setBulletsFired(I32 val) { _score.bulletsFired = val; }

////////////////////////////////////////////////////////////////////////////////
/// \fn void PlayerComponent::setBulletsHit(I32 val)
///
/// \brief  Sets the number of bullets that have hit.
///
/// \author Peter Bartosch
/// \date   2013-08-22
///
/// \param  val The value.
////////////////////////////////////////////////////////////////////////////////
void PlayerComponent::setBulletsHit(I32 val) { _score.bulletsHit = val; }
#pragma endregion scores

} //namespace scene
} //namespace pbj
