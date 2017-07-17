#include "weapon.hpp"

arda::Weapon::Weapon()
{
	CreateProperty<float>("ScatterRadiusVsInfantry");
	CreateProperty<float>("ScatterRadius");

	CreateProperty<float>("AttackRange");
	CreateProperty<float>("MinimumAttackRange");

	CreateProperty<int>("MinWeaponSpeed");
	CreateProperty<int>("WeaponSpeed");
	CreateProperty<int>("MaxWeaponSpeed");

	CreateProperty<int>("DelayBetweenShots");
	CreateProperty<int>("PreAttackDelay");
	CreateProperty<int>("FiringDuration");
	CreateProperty<std::string>("RadiusDamageAffects");
	CreateProperty<std::string>("PreAttackType");
	CreateProperty<bool>("ScaleWeaponSpeed");
	CreateProperty<int>("HitPercentage");
	CreateProperty<int>("PreAttackRandomAmount");
	CreateProperty<int>("RangeBonusMinHeight");
	CreateProperty<int>("RangeBonus");
	CreateProperty<int>("RangeBonusPerFoot");
	CreateProperty<int>("AcceptableAimDelta");
	CreateProperty<int>("ClipSize");
	CreateProperty<bool>("AutoReloadsClip");
	CreateProperty<bool>("InstantLoadClipOnActivate");
	CreateProperty<bool>("AutoReloadWhenIdle");
	CreateProperty<int>("ClipReloadTime");

	CreateProperty<int>("ContinuousFireOne");
	CreateProperty<int>("ContinuousFireCoast");
	CreateProperty<int>("CanFireWhileMoving");
	CreateProperty<int>("IdleAfterFiringDelay");
	CreateProperty<std::string>("ProjectileCollidesWith");

	CreateProperty<bool>("HitStoredTarget");
	CreateProperty<bool>("IsAimingWeapon");
	CreateProperty<bool>("AntiAirborneVehicle");
	CreateProperty<bool>("AntiAirborneMonster");
	CreateProperty<bool>("LeechRangeWeapon");
	CreateProperty<bool>("MeleeWeapon");
	CreateProperty<bool>("DamageDealtAtSelfPosition");
	CreateProperty<bool>("ShouldPlayUnderAttackEvaEvent");

	CreateProperty<std::string>("FireFX");
	CreateProperty<std::string>("FireFlankFX");
	CreateProperty<std::string>("PreAttackFX");
	CreateProperty<std::string>("FXTrigger");

	CreateProperty<std::string>("PreferredTargetBone");
	CreateProperty<bool>("ProjectileSelf");
	CreateProperty<std::string>("HitPassengerPercentage");
	CreateProperty<bool>("CanBeDodged");
}

