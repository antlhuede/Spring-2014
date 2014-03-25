#include "meta\meta.h"

namespace meta {
object_container::~object_container()
{
  clear();
}
bool object_container::add(const string& name, const variant& obj)
{
  return add(name, obj.type(), obj.data());
}
bool object_container::add(const string& name, const type& type, const void* obj)
{
  if (m_lexicon.find(name) != m_lexicon.end())
    return false;

  m_lexicon[name] = m_objects.size();
  m_objects.push_back(variant(type, obj));

  return true;
}
bool object_container::get(const string& name, variant& obj) const
{
  return get(name, obj.type(), obj.data());
}
const variant& object_container::get(const string& name) const
{
  return m_objects[m_lexicon.find(name)->second];
}
bool object_container::get(const string& name, const type& type, void* obj) const
{
  auto it = m_lexicon.find(name);
  if (it == m_lexicon.end())
    return false;

  const variant& var = m_objects[it->second];
  if (var.type() != type)
    return false;

  type.copy(obj, var.data());

  return true;
}
bool object_container::has(const string& name) const
{
  return (m_lexicon.find(name) != m_lexicon.end());
}
void object_container::clear()
{
  m_lexicon.clear();
  m_objects.clear();
}
}