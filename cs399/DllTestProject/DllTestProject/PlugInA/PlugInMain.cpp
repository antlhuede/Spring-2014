#include "Precompiled.h"
#include "../DllTestProject/PlugIn.h"
#include "../DllShared/TestClass.h"

class PlugInA : public PlugIn
{
public:

  int mHealth;

  int mLives;

  PlugInA()
  {
	  mHealth = -100;
	  mLives = 9;
  }

  ~PlugInA()
  {

  }

  void AddLives(int lives)
  {
	  mLives += lives;
  }
  int GetLives()
  {
	  return mLives + 100;
  }


  cstr GetName() override
  {
    return "PlugInBoozle";
  }

};


Animal::Animal()
{
}

void Animal::Speak()
{
	bar.Print();
}


PlugIn*  CreatePlugIn(PlugInManager* manager)
{
  TestClass a;



  PlugIn* plugIn = new PlugInA();
 // manager->AddPlugIn(plugIn);
  return plugIn;
}

void PatchObject(PlugIn* plug)
{
   auto b = new PlugInA();
   memcpy(plug, b, sizeof(void*));
   delete b;
}