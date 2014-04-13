// This is the main DLL file.

#include "stdafx.h"
#include "ManagedCppPlugin.h"

class Animal
{
public:
  Animal()
  {
    mLives = 42;
  }

  int mLives;
};

public ref class ParentingTool : public CSharpPluginLoader::ITool
{
public:
  Animal* mBob;

  ParentingTool()
  {
    mBob = new Animal();
  }

  // Our destructor is actually called by the IDisposable.Dispose() interface (or by the garbage collector)
  ~ParentingTool()
  {
    delete mBob;
  }

  virtual property String^ Name
  { 
    public: String^ get() { return "Parenting Tool"; }
  }

  virtual int ComputeValue(int x, int y)
  {
    return mBob->mLives;
  }
};

		//string PickObject(int x, int y);