#pragma once

namespace meta {

template <class T> const type* typeof(const T& var);
template <class T> const type* typeof();
const type* typeof(const char_t* name);

template <class T> void PlacementNew(void* memory);
template <class T> void CopyMemory(void* dest, const void* source);
template <class T> void CallDestructor(void* object);
template <class T> void DeletePointer(void* object);
template <class T> void* CloneObject(const void* object);
template <class T> void* AllocMemory();
template <class T> void DeleteMemory();
template <class T> T& ConvertMemory(void* memory);
}