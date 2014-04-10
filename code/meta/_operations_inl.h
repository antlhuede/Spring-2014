#pragma once

namespace meta
{
template <class T> const type* typeof(const T& var) { return internal::meta_holder<std::remove_cv<std::remove_pointer<std::remove_reference<T>::type>::type>::type>::s_pType; }
template <class T> const type* typeof() { return internal::meta_holder<std::remove_cv<std::remove_pointer<std::remove_reference<T>::type>::type>::type>::s_pType; }
inline const type* typeof(const char_t* name) { return internal::meta_registry::find(name); }

template <class T> void PlacementNew(void* memory)
{
  new (memory) T();
}
template <class T> void CopyMemory(void* dest, const void* source)
{
  new (dest) T(*static_cast<const T*>(source));
}
template <class T> void CallDestructor(void* object)
{
  static_cast<T*>(object)->~T();
}
template <class T> void DeletePointer(void* object)
{
  delete static_cast<T*>(object);
}
template <class T> void* CloneObject(const void* object)
{
  return new T(*static_cast<const T*>(object));
}
template <class T> void* AllocMemory()
{
  return new T;
}
template <class T> void DeleteMemory(void* memory)
{
  delete static_cast<T*>(memory);
}
template <class T> void AssignMemory(void* dest, const void* source)
{
  *static_cast<T*>(dest) = *static_cast<const T*>(source);
}
template <class T> T& ConvertMemory(void* memory)
{
  return *static_cast<T*>(memory);
}
}