////////////////////////////////////////////////////////////////////////////////
/// \file   Z:\Documents\PBJsimple\include\pbj\scene\player_component.h
///
/// \brief  Declares the player component class.
////////////////////////////////////////////////////////////////////////////////
#ifndef PBJ_SCENE_PLAYER_COMPONENT_H_
#define PBJ_SCENE_PLAYER_COMPONENT_H_

#include "pbj/_pbj.h"
#include "be/id.h"

using be::Id;

namespace pbj {
namespace scene {

class Entity;

////////////////////////////////////////////////////////////////////////////
/// \struct PlayerStats
///
/// \brief  Keeps track of statistics such as health and ammo for a
///         PlayerComponent.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////
struct PlayerStats
{
    I32 health;
    I32 maxHealth;
    I32 fuelRemaining;
    I32 maxFuel;
    I32 ammoRemaining;
    I32 maxAmmo;
    F32 rateOfFire;
    F32 reloadTime;
    F32 jumpSpeed;
    F32 moveSpeed;
    F32 thrust;

    PlayerStats()
    {
        health = 1000;
        maxHealth = 1000;
        fuelRemaining = 6000;
        maxFuel = 6000;
        ammoRemaining = 20;
        maxAmmo = 20;
        rateOfFire = 0.025f;
        reloadTime = 1.5f;
        jumpSpeed = 20.0f;
        moveSpeed = 15.0f;
        thrust = 500.0f;
    }
};

////////////////////////////////////////////////////////////////////////////
/// \struct PlayerScore
///
/// \brief  Tracks scoring data for a PlayerComponent.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////
struct PlayerScore
{
    I32 kills;
    I32 deaths;
    I32 bulletsFired;
    I32 bulletsHit;

    PlayerScore()
    {
        kills = 0;
        deaths = 0;
        bulletsFired = 0;
        bulletsHit = 0;
    }
};

////////////////////////////////////////////////////////////////////////////
/// \class  PlayerComponent
///
/// \brief  Represents a player in the game.  Executes actions like moving,
///         jumping, and shooting for the Entity it is attached to as well
///         as keeps track of stats for the player.
///
/// \author Peter Bartosch
/// \date   2013-08-22
////////////////////////////////////////////////////////////////////////////
class PlayerComponent
{
public:
    PlayerComponent(Id, PlayerStats, void*);
    ~PlayerComponent();

    PlayerStats getStats() const;
    void resetStats();

    Id getId() const;

    I32 getHealth() const;
    I32 getMaxHealth() const;
    I32 getFuelRemaining() const;
    I32 getMaxFuel() const;
    I32 getAmmoRemaining() const;
    I32 getMaxAmmo() const;
    F32 getRateOfFire() const;
    F32 getJumpSpeed() const;
    F32 getMoveSpeed() const;
    F32 getThrust() const;

    void setHealth(I32);
    void setMaxHealth(I32);
    void setFuelRemaining(I32);
    void setMaxFuel(I32);
    void setAmmoRemaining(I32);
    void setMaxAmmo(I32);
    void setRateOfFire(F32);
    void setJumpSpeed(F32);
    void setMoveSpeed(F32);
    void setThrust(F32);


    I32 getKills() const;
    I32 getDeaths() const;
    I32 getBulletsFired() const;
    I32 getBulletsHit() const;

    void setKills(I32);
    void setDeaths(I32);
    void setBulletsFired(I32);
    void setBulletsHit(I32);

    Entity* getOwner() const;

    bool canJump() const;
    void disableJump();
    void enableJump();

    bool isThrusting() const;
    void startThrust();
    void endThrust();

    void doThrust();
    void regenFuel();

    F32 getReloadTimer() const;
    F32 getFireTimer() const;
    bool fireOnCooldown() const;
    bool canShoot() const;
    bool reloading() const;
    void stepReloadTimer(F32);
    void resetReloadTimer();
    void stepFireTimer(F32);
    void fire(F32, F32);

    void moveLeft();
    void moveRight();
    void jump();

    void takeDamage(I32);
    void stop();

    bool isDead();
    F64 getTimeOfDeath();
    void setTimeOfDeath(F64);
private:
    Id _id;
    Entity* _owner;
    PlayerStats _stats;
    PlayerScore _score;
    bool _canJump;
    bool _thrusting;
    bool _forceFullRegen;
    bool _canShoot;
    bool _reloading;
    bool _fireCooldown;
    F32 _fireTimer;
    F32 _reloadTimer;
    F32 _thrustExtraDelay;
    F64 _timeOfDeath;
};

} //namespace scene
} //namespace pbj

#endif
