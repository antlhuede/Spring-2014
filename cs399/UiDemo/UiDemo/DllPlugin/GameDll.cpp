#include "GameDll.h"
#include "../Source/Startup.h"
#include "../Source/Core.h"
#include "../Source/WindowsSystem.h"
#include "Interprocess.h"

namespace Framework
{
  LRESULT WINAPI MessageHandler(HWND hWnd, UINT msg, WPARAM wParam,LPARAM lParam);
}

Framework::CoreEngine* engine = NULL;

HWND CreateGame(HWND parentWindow)
{
  engine = Framework::CreateGame(parentWindow);

  return Framework::WINDOWSSYSTEM->hWnd;
}

void DestroyGame()
{
  Framework::ShutdownGame();
}


void UpdateGame()
{
  engine->SingleUpdate();
}

bool SignalErrorHandler(const char * exp, const char * file, int line, const char * msg , ...)
{
  return false;
}


void DebugPrintHandlerVa(const char * format, va_list args)
{

  int textSize = _vscprintf(format, args) + 1;
  int messageSize = textSize + EditorMessageSize;
  EditorMessage* message = (EditorMessage*)alloca(messageSize);

  message->MessageSize = messageSize;
  message->MessageType = DebugConsole;

  vsnprintf_s(message->MessageData, textSize , _TRUNCATE , format, args);


  SendEditorMessage(message);
}


void DebugPrintHandler(const char * format, ... )
{
  va_list args;
  va_start(args, format);
  DebugPrintHandlerVa(format, args);
  va_end(args);
}

LRESULT DLLEXPORT WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  return Framework::MessageHandler(hWnd, msg, wParam, lParam);
}
