#pragma once

namespace meta
{
template <class T> const type& typeof(const T& var) { return *internal::meta_holder<std::remove_cv<T>::type>::s_pType; }
template <class T> const type& typeof() { return *internal::meta_holder<std::remove_cv<T>::type>::s_pType; }
inline const type& typeof(const char_t* name) { return *internal::meta_registry::find(name); }

template <class T> void PlacementNew(void* memory)
{
  new (memory)T();
}
template <class T> void CopyMemory(void* dest, const void* source)
{
  new (dest)T(*((T*)source));
}
template <class T> void CallDestructor(void* object)
{
  ((T*)object)->~T();
}
template <class T> void* CloneObject(const void* object)
{
  return new T(*((T*)object));
}
template <class T> void* AllocMemory()
{
  return new T;
}
}