#pragma once

namespace meta
{
template <class T> const type& typeof(const T& var) { return meta_holder<std::remove_cv<T>::type>::t; }
template <class T> const type& typeof() { return meta_holder<std::remove_cv<T>::type>::t; }
inline const type& typeof(const char_t* name) { return meta_registry::find(name); }

template <class T>
void PlacementNew(void* memory)
{
  new (memory)T();
}
template <class T>
void CopyMemory(void* dest, void* source)
{
  new (dest)T(*((T*)source));
}
template <class T>
void CallDestructor(void* object)
{
  ((T*)object)->~T();
}
}