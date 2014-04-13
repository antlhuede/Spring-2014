#include "../Source/Precompiled.h"
#define DLLEXPORT  __declspec(dllexport) _cdecl

extern "C"
{

HWND DLLEXPORT CreateGame(HWND parentWindow);
void DLLEXPORT DestroyGame();
void DLLEXPORT UpdateGame();
LRESULT DLLEXPORT WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

};
