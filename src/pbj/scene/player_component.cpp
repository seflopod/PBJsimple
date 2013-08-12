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
}

PlayerComponent::~PlayerComponent()
{
	_owner = 0;
}

PlayerStats PlayerComponent::getStats() const {	return _stats; }
void PlayerComponent::resetStats() { _stats = PlayerStats(); }

U32 PlayerComponent::getHealth() const { return _stats.health; }
U32 PlayerComponent::getMaxHealth() const { return _stats.maxHealth; }
U32 PlayerComponent::getFuelRemaining() const { return _stats.fuelRemaining; }
U32 PlayerComponent::getMaxFuel() const { return _stats.maxFuel; }
U32 PlayerComponent::getAmmoRemaining() const { return _stats.ammoRemaining; }
U32 PlayerComponent::getMaxAmmo() const { return _stats.maxAmmo; }
F32 PlayerComponent::getRateOfFire() const { return _stats.rateOfFire; }
F32 PlayerComponent::getJumpHeight() const { return _stats.jumpHeight; }
F32 PlayerComponent::getMoveSpeed() const { return _stats.moveSpeed; }

void PlayerComponent::setHealth(U32 val) { _stats.health = val; }
void PlayerComponent::setMaxHealth(U32 val) { _stats.maxHealth = val; }
void PlayerComponent::setFuelRemaining(U32 val) { _stats.fuelRemaining = val; }
void PlayerComponent::setMaxFuel(U32 val) { _stats.maxFuel = val; }
void PlayerComponent::setAmmoRemaining(U32 val) { _stats.ammoRemaining = val; }
void PlayerComponent::setMaxAmmo(U32 val) { _stats.maxAmmo = val; }
void PlayerComponent::setRateOfFire(F32 val) { _stats.rateOfFire = val; }
void PlayerComponent::setJumpHeight(F32 val) { _stats.jumpHeight = val; }
void PlayerComponent::setMoveSpeed(F32 val) { _stats.moveSpeed = val; }

void* PlayerComponent::getOwner() const { return _owner; }

} //namespace scene
} //namespace pbj