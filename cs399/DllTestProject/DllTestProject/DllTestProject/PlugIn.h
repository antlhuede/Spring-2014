#include "Standard.h"

#ifdef PLUGIN_EXPORTS
#define PLUGIN_DECL __declspec(dllexport)
#else
#define PLUGIN_DECL __declspec(dllimport)
#endif

#include "PlugInManager.h"


class PlugIn
{
public:
  virtual ~PlugIn(){};
  virtual cstr GetName() = 0;

  virtual void AddLives(int lives) = 0;
  virtual int GetLives() = 0;
};

static int Doozle = 10;

class Foo
{
public:
	void Print()
	{
		printf("%d, %p\n", Doozle, &Foo::Print);
		++Doozle;
	}
	int health;
};

class PLUGIN_DECL Animal
{
public:
	Animal();
	void Speak();
	int lives;

	Foo bar;
};

typedef PlugIn* (_cdecl* CreatePlugInFunc)(PlugInManager* manager);
typedef void (_cdecl* PatchObjectFunc)(PlugIn* plug);

extern "C"
{
  PLUGIN_DECL PlugIn* _cdecl CreatePlugIn(PlugInManager* manager);
  PLUGIN_DECL void _cdecl PatchObject(PlugIn* plug);
};

