#ifndef PLAYER_COMPONENT_H_
#define PLAYER_COMPONENT_H_

#include "pbj/_pbj.h"

namespace pbj {
namespace scene {

class Entity;


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

class PlayerComponent
{
public:
	PlayerComponent(PlayerStats, Entity*);
	~PlayerComponent();

	PlayerStats getStats() const;
	void resetStats();

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
	Entity* _owner;
	PlayerStats _stats;
	bool _canJump;
	bool _thrusting;
	bool _forceFullRegen;
	bool _canShoot;
	bool _reloading;
	bool _fireCooldown;
	F32 _fireTimer;
	F32 _reloadTimer;
	F64 _timeOfDeath;
};

} //namespace scene
} //namespace pbj

#endif
