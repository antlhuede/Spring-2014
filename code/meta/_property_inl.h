#pragma once

namespace meta {
template <class T, class U>
inline property::property(const string& name, T(U::*get)()const, void(U::*set)(T))
  : m_name(name), m_hasGetter(get != nullptr), m_hasSetter(set != nullptr)
  , m_get(get ? get : meta::function()), m_set(set ? set : meta::function())
{
  assert(get || set);

  if (get)
    assert(m_get.traits().numArguments == 0);

  if (set)
  {
    assert(m_set.traits().numArguments == 1);
    assert(m_set.traits().hasReturnValue == false);
  }

  if (get && set)
  {
    assert(m_get.traits().classType == m_set.traits().classType);
    assert(m_get.traits().returnType == m_set.traits().args[0].type);
  }

  m_classType = get ? m_get.traits().classType : m_set.traits().classType;
  m_type = get ? m_get.traits().returnType : m_set.traits().args[0].type;
}

template <class T, class U>
void property::set(U* object, const T& value) const
{
  assert(typeof<U>() == m_classType);
  assert(typeof<T>() == m_type);
  assert(m_hasSetter);
  m_set.call(object, std::forward<const T&>(value));
}
template <class U>
variant property::get(U* object) const
{
  assert(typeof<U>() == m_classType);
  assert(m_hasGetter);
  return m_get.call(object);
}

template <class T>
void property::set(void* object, const T& value) const
{
  assert(typeof<T>() == m_type);
  assert(m_hasSetter);
  m_set.call(object, std::forward<const T&>(value));
}
inline variant property::get(const void* object) const
{
  assert(m_hasGetter);
  return m_get.call(object);
}

template <class T, class U>
T property::get_as(U* object) const
{
  return get(object).get_as<T>();
}
}