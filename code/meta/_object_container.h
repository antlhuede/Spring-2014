#pragma once

namespace meta {
class object_container
{
public:
  bool add(const string& name, const variant& obj);

  bool get(const string& name, variant& obj) const;

  bool has(const string& name) const;

  void clear();

  const vector<variant>& objects() const { return m_objects; }
  const hash_map<string, size_t>& names() const { return m_names; }

  template <class T> bool add(const string& name, const T& obj);
  template <class T> bool get(const string& name, T& obj) const;
protected:
  hash_map<string, size_t> m_names;
  vector<variant> m_objects;
};

template <class T> 
bool object_container::add(const string& name, const T& obj)
{
  if (has(name) == true) 
    return false;

  m_names[name] = m_objects.size();
  m_objects.push_back(obj);

  return true;
}
template <class T> 
bool object_container::get(const string& name, T& obj) const
{
  auto it = m_names.find(name);
  if (it == m_names.end())
    return false;

  auto item = m_objects[it->second];
  if (item.type() != typeof<T>())
    return false;

  obj = item.get_as<T>();
  return true;
}
}