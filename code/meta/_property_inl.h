#pragma once

namespace meta {
template <class T, class U>
inline property::property(const string& name_, T(U::*get)()const, void(U::*set)(T))
  : name(name_), hasGetter(true), hasSetter(true)
  , m_get(get), m_set(set)
  , classType(typeof<U>())
  , type(typeof<T>()) {}

template <class T, class U>
property::property(const string& name_, T(U::*get)()const)
  : name(name_), hasGetter(true), hasSetter(false)
  , m_get(get), m_set(meta::function())
  , classType(typeof<U>())
  , type(typeof<T>()) {}

template <class T, class U>
property::property(const string& name_, void(U::*set)(T))
  : name(name_), hasGetter(false), hasSetter(true)
  , m_get(meta::function()), m_set(set)
  , classType(typeof<U>())
  , type(typeof<T>()) {}

template <class T, class U>
void property::set(U* object, const T& value) const
{
  assert(typeof<U>() == classType);
  assert(typeof<T>() == type);
  assert(hasSetter);
  m_set.Call(object, std::forward<const T&>(value));
}
template <class T>
void property::set(void* object, const T& value) const
{
  assert(typeof<T>() == type);
  assert(hasSetter);
  m_set.Call(object, std::forward<const T&>(value));
}
inline void property::set(const meta::type* classType, void* object, const meta::type* propertyType, const void* propertyValue)
{
  assert(classType == classType);
  assert(propertyType == type);
  m_set.CallGeneric(classType, object, 1, &propertyType, &propertyValue);
}

template <class U>
variant property::get(U* object) const
{
  assert(typeof<U>() == classType);
  assert(hasGetter);
  return m_get.Call(object);
}
inline variant property::get(const void* object) const
{
  assert(hasGetter);
  return m_get.Call(object);
}

template <class T, class U>
T property::get_as(U* object) const
{
  return get(object).get_as<T>();
}
}