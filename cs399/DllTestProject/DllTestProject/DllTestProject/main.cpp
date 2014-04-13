#include "Standard.h"

#include "PlugIn.h"
#include "../DllShared/TestClass.h"

int main()
{
	// FIXED WAY
	//CreatePlugIn(NULL);



	// FIXED CLASS IMPORTING
	//auto bob = new Animal();
	//bob->Speak();

	//Foo f;
	//f.Print();

	malloc(10);

	auto module = LoadLibrary("PluginA.dll");
	auto creator = (CreatePlugInFunc)GetProcAddress(module, "CreatePlugIn");

	PlugIn* plugin = creator(NULL);

	cstr name = plugin->GetName();
	printf("Name: %s\n", name);
	plugin->AddLives(5);
	printf("Lives: %d\n", plugin->GetLives());

	//string savedData = plugin->Save();

	delete plugin;

	FreeLibrary(module);
	getchar();


	//module = LoadLibrary("PluginA.dll");
	//creator = (CreatePlugInFunc)GetProcAddress(module, "CreatePlugIn");

	//PlugIn* tempOnlyForVTable = creator(NULL);
	//void* newVTable = *(void**)tempOnlyForVTable;
	//delete tempOnlyForVTable;

	//(*(void**)plugin) = newVTable;

	////plugin->Load(savedData);

	//name = plugin->GetName();
	//printf("Name: %s\n", name);
	//printf("Lives: %d\n", plugin->GetLives());
	//delete plugin;

	//FreeLibrary(module);


/*
	auto module = LoadLibrary("PluginA.dll");
	auto creator = (CreatePlugInFunc)GetProcAddress(module, "CreatePlugIn");

	creator(NULL);

	FreeLibrary(module);
	module = LoadLibrary("PluginA.dll");

	creator(NULL);*/
}

//
//int main()
//{
//  // Auto Style
//  // PlugIn* myPlug =  CreatePlugIn();
//
//  TestClass b;
//
//  b.GetValue();
//
//  HMODULE lib = GetModuleHandle("DllShared");
//  FreeLibrary(lib);
//
//    b.GetValue();
//
//  PlugInManager m;
//  HMODULE myLib = LoadLibrary("PlugInA");
//  CreatePlugInFunc createPlug = (CreatePlugInFunc)GetProcAddress(myLib, "CreatePlugIn");
//  PlugIn* myPlug = createPlug(&m);
//  printf("Loaded Plugin %s\n", myPlug->GetName());
//
//
//
//  /*
//  printf("Load?\n");
//  getchar();
//
//
//  HMODULE myLib = LoadLibrary("PlugInA");
//  CreatePlugInFunc createPlug = (CreatePlugInFunc)GetProcAddress(myLib, "CreatePlugIn");
//  PlugIn* myPlug = createPlug();
//  printf("Loaded Plugin %s\n", myPlug->GetName());
//  
//
//  printf("Reload?\n");
//  FreeLibrary(myLib);
//  getchar();
//
//
//
//  HMODULE myLib2 = LoadLibrary("PlugInA");
//  
//  createPlug = (CreatePlugInFunc)GetProcAddress(myLib2, "CreatePlugIn");
//  PatchObjectFunc patchFunc = (PatchObjectFunc)GetProcAddress(myLib2, "PatchObject");
//
//  patchFunc(myPlug);
//
//  printf("Loaded Plugin %s\n", myPlug->GetName());
//
//
//  */
//
//
//
//  //HMODULE lib = GetModuleHandle("PlugInA");
//  //BOOL success = FreeLibrary(lib);
//  //  success = FreeLibrary(lib);
//  //if(!success)
//  //  __debugbreak();
//
////f("Loaded Plugin %s\n", myPlug->GetName());
////  LoadLibrary()
//
//  //delete myPlug;
//
//}
