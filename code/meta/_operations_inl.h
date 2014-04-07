#pragma once

namespace meta
{
template <class T> const type& typeof(const T& var) { return *internal::meta_holder<std::remove_cv<std::remove_reference<T>::type>::type>::s_pType; }
template <class T> const type& typeof() { return *internal::meta_holder<std::remove_cv<std::remove_reference<T>::type>::type>::s_pType; }
inline const type& typeof(const char_t* name) { return *internal::meta_registry::find(name); }

template <class T> void PlacementNew(void* memory)
{
  new (memory) T();
}
template <class T> void CopyMemory(void* dest, const void* source)
{
  new (dest) T(*reinterpret_cast<const T*>(source));
}
template <class T> void CallDestructor(void* object)
{
  reinterpret_cast<T*>(object)->~T();
}
template <class T> void DeletePointer(void* object)
{
  delete reinterpret_cast<T*>(object);
}
template <class T> void* CloneObject(const void* object)
{
  return new T(*reinterpret_cast<const T*>(object));
}
template <class T> void* AllocMemory()
{
  return new T;
}
template <class T> void DeleteMemory(void* memory)
{
  delete reinterpret_cast<T*>(memory);
}
template <class T> void AssignMemory(void* dest, const void* source)
{
  *reinterpret_cast<T*>(dest) = *reinterpret_cast<const T*>(source);
}
}