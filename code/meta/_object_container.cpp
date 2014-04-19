#include "meta\meta.h"

namespace meta {
bool object_container::add(const string& name, const variant& obj)
{
  if (m_names.find(name) != m_names.end())
    return false;

  m_names[name] = m_objects.size();
  m_objects.push_back(obj);
  return true;
}
bool object_container::get(const string& name, variant& obj) const
{
  auto item = m_names.find(name);
  if (item == m_names.end())
    return false;

  obj = m_objects[item->second];
  return true;
}
bool object_container::has(const string& name) const
{
  return (m_names.find(name) != m_names.end());
}
void object_container::clear()
{
  m_names.clear();
  m_objects.clear();
}
}