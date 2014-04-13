
#pragma once

extern "C"
{
  __declspec(dllexport) void*       __cdecl IToolCreate();
  __declspec(dllexport) void        __cdecl IToolDestroy(void* instance);

  __declspec(dllexport) const char* __cdecl IToolGetName(void* instance);
  __declspec(dllexport) int         __cdecl IToolComputeValue(void* instance, int x, int y);
}
