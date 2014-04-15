#pragma once

namespace meta {
template <class U, class T>
inline property::property(const string& name, T(U::*get)()const, void(U::*set)(T))
  : m_name(name), m_get(get), m_set(set)
{
  assert(m_get.traits().classType == m_set.traits().classType);

  assert(m_get.traits().numArguments == 0);
  assert(m_set.traits().numArguments == 1);
  assert(m_get.traits().returnType == m_set.traits().args[0].type);
  assert(m_set.traits().hasReturnValue == false);

  m_classType = m_get.traits().classType;
  m_type = m_get.traits().returnType;
}

template <class U, class T>
void property::set(U* object, const T& value) const
{
  assert(typeof<U>() == m_classType);
  assert(typeof<T>() == m_type);
  m_set.call(object, value);
}
template <class U>
variant property::get(U* object) const
{
  assert(typeof<U>() == m_classType);
  return m_get.call(object);
}
}