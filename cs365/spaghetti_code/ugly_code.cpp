#include "uglycode.h"

const float Monster::MAX_FIRE_DURATION = 2.f;
const float Monster::MAX_FREEZE_DURATION = 2.f;
const float Monster::MAX_INVINCIBILITY_DURATION = 4.f;
const float Monster::MAX_UNDEAD_INVINCIBILITY_DURATION = MAX_INVINCIBILITY_DURATION / 2.f; //undead lasts half as long

Monster::Monster(int hitpoints_,
                 int maxHitpoints_,
                 bool isDead_,
                 bool isUndead_,
                 bool onFire_,
                 bool isFrozen_,
                 float fireDuration_,
                 float freezeDuration_,
                 int fireDamagePerSecond_,
                 bool resistsFire_,
                 bool resistsIce_,
                 bool resistsLighting_,
                 bool invincibilityActive_,
                 float invicibilityDuration_,
                 int armorPoints_,
                 bool hasArmor_,
                 bool hitpointsRegenerate_,
                 int hitpointRegenerationAmount_)
  : hitpoints(hitpoints_),
    maxHitpoints(maxHitpoints_),
    isDead(isDead_),
    isUndead(isUndead_),
    onFire(onFire_),
    isFrozen(isFrozen_),
    fireDuration(fireDuration_),
    freezeDuration(freezeDuration_),
    fireDamagePerSecond(fireDamagePerSecond_),
    resistsFire(resistsFire_),
    resistsIce(resistsIce_),
    resistsLighting(resistsLighting_),
    invincibilityActive(invincibilityActive_),
    invicibilityDuration(invicibilityDuration_),
    armorPoints(armorPoints_),
    hasArmor(hasArmor_),
    hitpointsRegenerate(hitpointsRegenerate_),
    hitpointRegenerationAmount(hitpointRegenerationAmount_)
{

}

// Update is called every pass through the game loop 
// elapsedTime is a positive value that represents the
// amount of time since Update was last called
void Monster::Update(float elapsedTime)
{
  if (onFire)
  {
    fireDuration += elapsedTime;
    hitpoints -= (int)((float)fireDamagePerSecond * elapsedTime);
    if (fireDuration > MAX_FIRE_DURATION) {
      onFire = false;
    }
  }
  else if (isFrozen)
  {
    freezeDuration += elapsedTime;
    if (freezeDuration > MAX_FREEZE_DURATION)
      isFrozen = false;
  }

  if (invincibilityActive)
  {
    invicibilityDuration += elapsedTime;
    if (invicibilityDuration > MAX_INVINCIBILITY_DURATION && !isUndead) {
      invicibilityDuration = 0.f;
      invincibilityActive = false;
    }
    else if (isUndead && invicibilityDuration > MAX_UNDEAD_INVINCIBILITY_DURATION)
    {
      // undead invincibility lasts less time
      invicibilityDuration = 0.f;
      invincibilityActive = false;
    }
  }

  if (hitpointsRegenerate && !onFire && !isFrozen) {
    hitpoints += hitpointRegenerationAmount;
    if (hitpoints>maxHitpoints)
      hitpoints = maxHitpoints;
  }
}

// called when an attack happens
void Monster::DoDamage(int amount,
                       bool damageIsFire,
                       bool damageisIce,
                       bool damageIsLightning,
                       bool damageIgnoresResistance,
                       bool destroysArmorOnly)
{

  if (destroysArmorOnly)
  {
    armorPoints = 0;
  }
  // invincible monsters can't take damage from any source
  if (invincibilityActive)
  {
    return;
  }

  if (!damageIgnoresResistance)
  {
    if (damageIsFire && resistsFire)
    {
      return;
    }
    else if (damageisIce && resistsIce)
      return;
    else if (damageIsLightning && resistsLighting)
      return;
  }

  // fix for bug #145783: don't allow negative amounts
  // even though damage heals undead enemies
  if (amount < 0)
    return;

  // HACK: undead enemies from bug #145783
  if (isUndead && !damageIsFire)  {
    hitpoints += amount;
    if (hitpoints > maxHitpoints)
    {
      hitpoints = maxHitpoints;
    }
    return;
  }
  else if (damageIsFire && isUndead)
  {
    // fire does double damage
    amount *= 2;
    fireDamagePerSecond = amount * 2;
    onFire = true;
    fireDuration = 0.f;

    if (isFrozen)
      isFrozen = false;
  }
  else if (damageIsFire && !isUndead)
  {
    onFire = true;
    fireDamagePerSecond = amount;
    if (isFrozen)
      isFrozen = false;
  }

  // fire melts frozen but still does damage
  if ((damageIsFire && isFrozen && !isUndead) || damageIgnoresResistance)
  {
    isFrozen = false;
    onFire = true;
    fireDamagePerSecond = amount;
  }

  // resets freeze duration but doesn't 
  // do additional damage for ice
  if (damageisIce && isFrozen && !onFire)
  {
    freezeDuration = 0.f;
    return;
  }
  else if (damageisIce && !onFire)
  {
    freezeDuration = 0.f;
    isFrozen = true;
  }

  if (damageIsLightning && resistsLighting && !damageIgnoresResistance)
  {
    return;
  }
  else if (damageIsLightning && !resistsLighting && damageIgnoresResistance)
  {
    hitpoints -= amount;
    if (hitpoints <= 0)
    {
      hitpoints = 0;
      isDead = true;
    }
    return;
  }

  // lightning bypasses armor
  // also, armor counts as "resistance" for 
  // purposes of attacks that ignore resistance
  if (hasArmor && armorPoints > 0 && !damageIsLightning && !damageIgnoresResistance)
  {
    armorPoints -= amount;
    if (armorPoints < 0)
    {
      // armor gone but there's still
      // damage then it should overflow into
      // hitpoints
      amount = abs(armorPoints);
      armorPoints = 0;
    }
    else
    {
      // armor absorbed all damage
      return;
    }
  }

  hitpoints -= amount;
  if (hitpoints <= 0)
  {
    hitpoints = 0;
    isDead = true;
  }
}