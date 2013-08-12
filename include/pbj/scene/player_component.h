#ifndef PLAYER_COMPONENT_H_
#define PLAYER_COMPONENT_H_

#include "pbj/_pbj.h"

namespace pbj
{
namespace scene
{
	struct PlayerStats
	{
		U32 health;
		U32 maxHealth;
		U32 fuelRemaining;
		U32 maxFuel;
		U32 ammoRemaining;
		U32 maxAmmo;
		F32 rateOfFire;
		F32 jumpHeight;
		F32 moveSpeed;

		PlayerStats()
		{
			health = 1000;
			maxHealth = 1000;
			fuelRemaining = 5000;
			maxFuel = 5000;
			ammoRemaining = 10000;
			maxAmmo = 10000;
			rateOfFire = 0.1f;
			jumpHeight = 5.0f;
			moveSpeed = 10.0f;
		}
	};

	class PlayerComponent
	{
	public:
		PlayerComponent(PlayerStats, void*);
		~PlayerComponent();

		PlayerStats getStats() const;
		void resetStats();

		U32 getHealth() const;
		U32 getMaxHealth() const;
		U32 getFuelRemaining() const;
		U32 getMaxFuel() const;
		U32 getAmmoRemaining() const;
		U32 getMaxAmmo() const;
		F32 getRateOfFire() const;
		F32 getJumpHeight() const;
		F32 getMoveSpeed() const;

		void setHealth(U32);
		void setMaxHealth(U32);
		void setFuelRemaining(U32);
		void setMaxFuel(U32);
		void setAmmoRemaining(U32);
		void setMaxAmmo(U32);
		void setRateOfFire(F32);
		void setJumpHeight(F32);
		void setMoveSpeed(F32);

		void* getOwner() const;

	private:
		void* _owner;
		PlayerStats _stats;
	};
} //namespace scene
} //namespace pbj
#endif