

#include "Precompiled.h"
#include "Core.h"
#include "WindowsSystem.h"
#include "Graphics.h"
#include "Physics.h"
#include "GameLogic.h"

//The title the window will have at the top
const char windowTitle[] = "Engine";
const int ClientWidth = 800;
const int ClientHeight = 600;

namespace Framework
{

CoreEngine* engine = NULL;

CoreEngine* CreateGame(HWND parent)
{
  DebugPrint("Starting Game\n");
  OutputDebugStringA("awwwwwwwwwwwwwwwwwwwwwwwwwwwwww\n");

  //Initialize the game by creating the various systems needed and adding them to the core engine
  WindowsSystem* windows = new WindowsSystem(parent, windowTitle, ClientWidth, ClientHeight);

  //Create the core engine which manages all the systems that make up the game
  engine = new CoreEngine();


  Graphics* graphics = new Graphics();
  graphics->SetWindwProperties(windows->hWnd, ClientWidth, ClientHeight);

  engine->AddSystem(windows);
  engine->AddSystem(new GameObjectFactory());
  engine->AddSystem(graphics);
  engine->AddSystem(new Physics());
  engine->AddSystem(new GameLogic());
  //TODO: Add additional systems, such as audio, and possibly xinput, lua, etc.

  engine->Initialize();


  //Everything is set up, so activate the window
  windows->ActivateWindow();

  return engine;
}


void ShutdownGame()
{
  //Delete all the game objects
  FACTORY->DestroyAllObjects();

  //Delete all the systems
  engine->DestroySystems();

  //Delete the engine itself
  delete engine;
}

}
