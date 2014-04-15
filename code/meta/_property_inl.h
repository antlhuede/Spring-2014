#pragma once

namespace meta {
inline property::property(const string& name, const function& get, const function& set)
: m_name(name), m_get(get), m_set(set)
{
  assert(get.traits().classType == set.traits().classType);

  assert(get.traits().numArguments == 0);
  assert(set.traits().numArguments == 1);
  assert(get.traits().returnType == set.traits().args[0].type);
  assert(set.traits().hasReturnValue == false);

  m_classType = get.traits().classType;
  m_type = get.traits().returnType;
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