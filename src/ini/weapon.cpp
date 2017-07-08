#include "weapon.hpp"

arda::Weapon::Weapon()
{
	FloatProperty("ScatterRadiusVsInfantry");
	FloatProperty("ScatterRadius");

	FloatProperty("AttackRange");
	
	FloatProperty("MinimumAttackRange");
	IntProperty("MinWeaponSpeed");
	IntProperty("WeaponSpeed");
	IntProperty("MaxWeaponSpeed");

	IntProperty("DelayBetweenShots");

	IntProperty("PreAttackDelay");

	IntProperty("FiringDuration");

	StringProperty("FireFX");
	StringProperty("RadiusDamageAffects");

	StringProperty("PreAttackType");

	BoolProperty("ScaleWeaponSpeed");

	IntProperty("HitPercentage");
	IntProperty("PreAttackRandomAmount");

	BoolProperty("IsAimingWeapon");
	BoolProperty("AntiAirborneVehicle");
	StringProperty("FXTrigger");
}

