#pragma once
#include <math.h>


// TODO: an attack needs to be added that turns a Monster to stone 
// temporarily.  While stone, the monster can't take any damage, even 
// if the damage is not resistable, but
// also can't regenerate or do anything else.  If it's frozen or
// on fire, then it should un-freeze and stop being on fire when 
// turned to stone.
class Monster
{
public:

	Monster(int hitpoints_,
	        int maxHitpoints_,
	        bool isDead_,
	        bool isUndead_,
	        bool onFire_,
	        bool isFrozen_,
	        // how long something has been on fire
	        float fireDuration_,
	        // how long something has been frozen
	        float freezeDuration_,

	        // amount of damage fire is doing per second
	        int fireDamagePerSecond_,

	        bool resistsFire_,
	        bool resistsIce_,
	        bool resistsLighting_,

	        bool invincibilityActive_,
	        float invicibilityDuration_,

	        int armorPoints_,
	        bool hasArmor_,
	        bool hitpointsRegenerate_,
	        int hitpointRegenerationAmount_);

	// Update is called every pass through the game loop 
	// elapsedTime is a positive value that represents the
	// amount of time since Update was last called
	void Update(float elapsedTime);

	// called when an attack happens
	void DoDamage(int amount, 
		            bool damageIsFire, 
		            bool damageisIce, 
		            bool damageIsLightning,
		            bool damageIgnoresResistance,
		            bool destroysArmorOnly);

  static const float MAX_FREEZE_DURATION;
  static const float MAX_FIRE_DURATION;
  static const float MAX_INVINCIBILITY_DURATION;
  static const float MAX_UNDEAD_INVINCIBILITY_DURATION;

// TODO: probably these should be encapsulated
// TODO: they were, but that made them untestable
// TODO: re-encapsulate them, this is really bad code
	int hitpoints;
	int maxHitpoints;
	bool isDead;
	bool isUndead;
	bool onFire;
	bool isFrozen;
	// how long something has been on fire
	float fireDuration;
	// how long something has been frozen
	float freezeDuration;

	// amount of damage fire is doing per second
	int fireDamagePerSecond;

	bool resistsFire;
	bool resistsIce;
	bool resistsLighting;

	bool invincibilityActive;
	float invicibilityDuration;

	int armorPoints;
	bool hasArmor;
	bool hitpointsRegenerate;
	int hitpointRegenerationAmount;
};