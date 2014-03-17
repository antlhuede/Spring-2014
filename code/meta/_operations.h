#pragma once

namespace meta {

template <class T> const type& typeof(const T& var);
template <class T> const type& typeof();
const type& typeof(const char_t* name);

template <class T> void PlacementNew(void* memory);
template <class T> void CopyMemory(void* dest, void* source);
template <class T> void CallDestructor(void* object);

}