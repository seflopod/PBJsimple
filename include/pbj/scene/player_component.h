#ifndef PLAYER_COMPONENT_H_
#define PLAYER_COMPONENT_H_

#include "pbj/_pbj.h"

namespace pbj
{
namespace scene
{
	struct PlayerStats
	{
		I32 health;
		I32 maxHealth;
		I32 fuelRemaining;
		I32 maxFuel;
		I32 ammoRemaining;
		I32 maxAmmo;
		F32 rateOfFire;
		F32 jumpSpeed;
		F32 moveSpeed;
		F32 thrust;

		PlayerStats()
		{
			health = 1000;
			maxHealth = 1000;
			fuelRemaining = 6000;
			maxFuel = 6000;
			ammoRemaining = 10000;
			maxAmmo = 10000;
			rateOfFire = 0.1f;
			jumpSpeed = 10.0f;
			moveSpeed = 10.0f;
			thrust = 1000.0f;
		}
	};

	class PlayerComponent
	{
	public:
		PlayerComponent(PlayerStats, void*);
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

		void* getOwner() const;

		bool canJump() const;
		void disableJump();
		void enableJump();

		bool isThrusting() const;
		void startThrust();
		void endThrust();
		//void continueThrust();
		
		void doThrust();
		void regenFuel();
	private:
		void* _owner;
		PlayerStats _stats;
		bool _canJump;
		bool _thrusting;
		bool _forceFullRegen;
	};
} //namespace scene
} //namespace pbj
#endif