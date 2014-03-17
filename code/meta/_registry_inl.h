#pragma once

namespace meta
{

inline const type& meta_registry::find(const string& key)
{
  auto it = m_registry.find(key);
  if (it == m_registry.end())
    return meta_holder<nulltype>::t;

  return *it->second;
}

inline bool meta_registry::register_type(const type* t)
{
  if (m_registry.find(t->name()) != m_registry.end())
    return false;

  m_registry[t->name()] = t;
  return true;
}

}