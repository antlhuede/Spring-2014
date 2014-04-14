
#include "meta\meta.h"
#include "gmock\gmock.h"
#include "spaghetti_code\uglycode.h"

int main(int argc, char** argv)
{
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

Monster* global_monster = nullptr;

Monster* create_monster();
Monster* create_default_monster();
void delete_monster();

void run_update_onFire_test(int hitpoints, int fire_dps, float time_step, float duration);
void run_update_isFrozen_test(float time_step, float duration);
void run_update_invincibilityActive_test(bool is_undead, float time_step, float duration);
void run_update_hitpointsRegenerate_test(int hitpointRegenerationAmount, int hitpoints, int maxHitpoints, float time_step, float duration);

TEST(Monster_Constructor, non_default_constructor)
{
  const int hitpoints = 100;
  const int maxHitpoints = 2000;
  const bool isDead = false;
  const bool isUndead = true;
  const bool onFire = false;
  const bool isFrozen = true;
  const float fireDuration = 23.0f;
  const float freezeDuration = 17.0f;
  const int fireDamagePerSecond = 2;
  const bool resistsFire = false;
  const bool resistsIce = true;
  const bool resistsLighting = false;
  const bool invincibilityActive = true;
  const float invicibilityDuration = 6.0f;
  const int armorPoints = 132;
  const bool hasArmor = false;
  const bool hitpointsRegenerate = true;
  const int hitpointRegenerationAmount = 4;

  Monster* monster = new Monster(hitpoints,
                                 maxHitpoints,
                                 isDead,
                                 isUndead,
                                 onFire,
                                 isFrozen,
                                 fireDuration,
                                 freezeDuration,
                                 fireDamagePerSecond,
                                 resistsFire,
                                 resistsIce,
                                 resistsLighting,
                                 invincibilityActive,
                                 invicibilityDuration,
                                 armorPoints,
                                 hasArmor,
                                 hitpointsRegenerate,
                                 hitpointRegenerationAmount);

#define CREATE_EQUALITY_TEST(VARIABLE) \
  EXPECT_EQ(monster->VARIABLE, VARIABLE) << "Constructor failed to assign " << #VARIABLE << " correctly";

  CREATE_EQUALITY_TEST(hitpoints)
  CREATE_EQUALITY_TEST(maxHitpoints)
  CREATE_EQUALITY_TEST(isDead)
  CREATE_EQUALITY_TEST(isUndead)
  CREATE_EQUALITY_TEST(onFire)
  CREATE_EQUALITY_TEST(isFrozen)
  CREATE_EQUALITY_TEST(fireDuration)
  CREATE_EQUALITY_TEST(freezeDuration)
  CREATE_EQUALITY_TEST(fireDamagePerSecond)
  CREATE_EQUALITY_TEST(resistsFire)
  CREATE_EQUALITY_TEST(resistsIce)
  CREATE_EQUALITY_TEST(resistsLighting)
  CREATE_EQUALITY_TEST(invincibilityActive)
  CREATE_EQUALITY_TEST(invicibilityDuration)
  CREATE_EQUALITY_TEST(armorPoints)
  CREATE_EQUALITY_TEST(hasArmor)
  CREATE_EQUALITY_TEST(hitpointsRegenerate)
  CREATE_EQUALITY_TEST(hitpointRegenerationAmount)
#undef CREATE_EQUALITY_TEST

  delete monster;
}

TEST(Monster_Update, onFire)
{
  //testing update with an elapsed time of 3 seconds
  run_update_onFire_test(3, 1, 3.0f, 3.0f);

  //testing update with an elapsed time of 100.0f seconds
  //update applies the damage before checking duration, so 100% of damage gets applied here
  run_update_onFire_test(100, 1, 100.0f, 100.0f);
  run_update_onFire_test(100, 1, 7.0f, 100.0f);

  //testing update with values that increment by 1
  run_update_onFire_test(100, 1, 1.0f, 100.0f);
}

TEST(Monster_Update, isFrozen)
{
  run_update_isFrozen_test(3.0f, 3.0f);
  run_update_isFrozen_test(100.0f, 100.0f);
  run_update_isFrozen_test(7.0f, 100.0f);
  run_update_isFrozen_test(1.0f, 100.0f);
}

TEST(Monster_Update, invincibilityActive)
{
  run_update_invincibilityActive_test(false, 1.0f, 100.0f);
  run_update_invincibilityActive_test(true, 1.0f, 100.0f);

  run_update_invincibilityActive_test(false, 7.0f, 100.0f);
  run_update_invincibilityActive_test(true, 7.0f, 100.0f);

  run_update_invincibilityActive_test(false, 13.0f, 100.0f);
  run_update_invincibilityActive_test(true, 13.0f, 100.0f);
}

TEST(Monster_Update, hitpointsRegenerate)
{
  run_update_hitpointsRegenerate_test(1, 0, 100, 1.0f, 100.0f);
  run_update_hitpointsRegenerate_test(10, 10, 100, 1.0f, 10.0f);
  run_update_hitpointsRegenerate_test(12, 0, 126, 7.0f, 100.0f);
  run_update_hitpointsRegenerate_test(3, 17, 132, 13.0f, 168.0f);
}

typedef void(*UpdateCallback)(Monster* monster, float elapsed_time, bool first_call);
void run_update(Monster* monster, float time_step, float duration, UpdateCallback callback);
bool check_monster_effect(bool monster_effect_active, float monster_effect_duration, float elapsed_time, float max_duration);

void check_on_fire(Monster* monster, float elapsed_time, bool first_call);
void check_still_frozen(Monster* monster, float elapsed_time, bool first_call);
void check_still_invincible(Monster* monster, float elapsed_time, bool first_call);
void check_hitpoint_regeneration(Monster* monster, float elapsed_time, bool first_call);

void run_update_onFire_test(int hitpoints, int fire_dps, float time_step, float duration)
{
  Monster* monster = create_default_monster();

  monster->hitpoints = hitpoints;
  monster->onFire = true;
  monster->fireDamagePerSecond = fire_dps;

  run_update(monster, time_step, duration, check_on_fire);

  //check amount of damage done
  //if timestep is less than max_duration, an extra step is applied as damage because duration check is done after damage application
  float damage_duration = (time_step <= Monster::MAX_FIRE_DURATION) ? (Monster::MAX_FIRE_DURATION + time_step) : time_step;
  EXPECT_EQ(monster->hitpoints, hitpoints - fire_dps * damage_duration) << "On Fire did incorrect amount of damage";
}

void run_update_isFrozen_test(float time_step, float duration)
{
  Monster* monster = create_default_monster();
  monster->isFrozen = true;
  run_update(monster, time_step, duration, check_still_frozen);
}

void run_update_invincibilityActive_test(bool is_undead, float time_step, float duration)
{
  Monster* monster = create_default_monster();
  monster->invincibilityActive = true;
  monster->isUndead = is_undead;
  run_update(monster, time_step, duration, check_still_invincible);
}

void run_update_hitpointsRegenerate_test(int hitpointRegenerationAmount, int hitpoints, int maxHitpoints, float time_step, float duration)
{
  Monster* monster = create_default_monster();
  monster->hitpointsRegenerate = true;
  monster->hitpointRegenerationAmount = hitpointRegenerationAmount;
  monster->hitpoints = hitpoints;
  monster->maxHitpoints = maxHitpoints;
  run_update(monster, time_step, duration, check_hitpoint_regeneration);
}

#define CHECK_EFFECT(EFFECT_ACTIVE, EFFECT_DURATION, ELAPSED_TIME, MAX_DURATION)  \
  static bool continue_checks = true;                                             \
  if (first_call) continue_checks = true;                                         \
  if (continue_checks)                                                            \
    continue_checks = check_monster_effect(EFFECT_ACTIVE, EFFECT_DURATION, elapsed_time, MAX_DURATION)

void check_on_fire(Monster* monster, float elapsed_time, bool first_call)
{
  CHECK_EFFECT(monster->onFire, monster->fireDuration, elapsed_time, Monster::MAX_FIRE_DURATION);
}
void check_still_frozen(Monster* monster, float elapsed_time, bool first_call)
{
  CHECK_EFFECT(monster->isFrozen, monster->freezeDuration, elapsed_time, Monster::MAX_FREEZE_DURATION);
}
void check_still_invincible(Monster* monster, float elapsed_time, bool first_call)
{
  float max_duration = monster->isUndead ? Monster::MAX_UNDEAD_INVINCIBILITY_DURATION : Monster::MAX_INVINCIBILITY_DURATION;
  CHECK_EFFECT(monster->invincibilityActive, monster->invicibilityDuration, elapsed_time, max_duration);
}
void check_hitpoint_regeneration(Monster* monster, float elapsed_time, bool first_call)
{
  static Monster* previous_monster = nullptr;
  static int previous_hitpoints = 0;

  if (first_call == false && monster->hitpointsRegenerate && monster->isFrozen == false && monster->onFire == false)
  {
    if (previous_hitpoints < monster->maxHitpoints)
      EXPECT_LT(previous_hitpoints, monster->hitpoints);
    if (previous_hitpoints + monster->hitpointRegenerationAmount >= monster->maxHitpoints)
      EXPECT_EQ(monster->hitpoints, monster->maxHitpoints);
  }

  previous_hitpoints = monster->hitpoints;
}

bool check_monster_effect(bool monster_effect_active, float monster_effect_duration, float elapsed_time, float max_duration)
{
  bool effect_continues = (elapsed_time <= max_duration) ? true : false;
  EXPECT_EQ(monster_effect_active, effect_continues);

  bool continue_checks = true;
  if (effect_continues && monster_effect_duration <= 0.0f)
  {
    EXPECT_GT(monster_effect_duration, 0.0f) << "Monster effect duration not accumulating correctly";
    continue_checks = false;
  }
  else if (effect_continues == false && monster_effect_duration != 0.0f)
  {
    EXPECT_EQ(monster_effect_duration, 0.0f) << "Monster effect duration not reset correctly";
    continue_checks = false;
  }
  return continue_checks;
}
void run_update(Monster* monster, float time_step, float duration, UpdateCallback callback)
{
  float elapsed_time = 0.0f;
  bool first_call = true;
  while (elapsed_time < duration)
  {
    monster->Update(time_step);
    elapsed_time += time_step;

    if (callback)
      callback(monster, elapsed_time, first_call);
    first_call = false;
  }
}

Monster* create_default_monster()
{
  const int hitpoints = 0;
  const int maxHitpoints = 0;
  const bool isDead = false;
  const bool isUndead = false;
  const bool onFire = false;
  const bool isFrozen = false;
  const float fireDuration = 0.0f;
  const float freezeDuration = 0.0f;
  const int fireDamagePerSecond = 0;
  const bool resistsFire = false;
  const bool resistsIce = false;
  const bool resistsLighting = false;
  const bool invincibilityActive = false;
  const float invicibilityDuration = 0.0f;
  const int armorPoints = 0;
  const bool hasArmor = false;
  const bool hitpointsRegenerate = false;
  const int hitpointRegenerationAmount = 0;


  if (global_monster == nullptr)
    atexit(delete_monster);
  else
    delete_monster();

  global_monster = new Monster(hitpoints,
                               maxHitpoints,
                               isDead,
                               isUndead,
                               onFire,
                               isFrozen,
                               fireDuration,
                               freezeDuration,
                               fireDamagePerSecond,
                               resistsFire,
                               resistsIce,
                               resistsLighting,
                               invincibilityActive,
                               invicibilityDuration,
                               armorPoints,
                               hasArmor,
                               hitpointsRegenerate,
                               hitpointRegenerationAmount);
  return global_monster;
}
void delete_monster()
{
  delete global_monster;
  global_monster = nullptr;
}