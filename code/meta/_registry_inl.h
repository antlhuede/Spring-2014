#pragma once

namespace meta
{
namespace internal {
inline const type* meta_registry::find(const string& key)
{
  auto it = m_registry->find(key);
  if (it == m_registry->end())
    return meta_holder<nulltype>::s_pType;

  return it->second;
}

inline bool meta_registry::register_type(const type* pType)
{
  if (m_registry->find(pType->name()) != m_registry->end())
    return false;

  (*m_registry)[pType->name()] = pType;
  return true;
}
}
}