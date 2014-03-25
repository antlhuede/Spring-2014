#pragma once

namespace meta {
class object_container
{
public:
  ~object_container();

  template <class T>
  bool add(const string& name, const T& obj);
  template <class T>
  bool get(const string& name, T& obj);
  template <class T>
  const T& get(const string& name);

  bool add(const string& name, const variant& obj);
  bool add(const string& name, const type& type, const void* obj);

  bool get(const string& name, variant& obj) const;
  bool get(const string& name, const type& type, void* obj) const;
  const variant& get(const string& name) const;

  bool has(const string& name) const;

  void clear();

  const vector<variant>& objects() const { return m_objects; }
  const hash_map<string, size_t>& lexicon() const { return m_lexicon; }

private:
  hash_map<string, size_t> m_lexicon;
  vector<variant> m_objects;
};

template <class T>
bool object_container::add(const string& name, const T& obj)
{
  return add(name, meta::typeof(obj), &obj);
}
template <class T>
bool object_container::get(const string& name, T& obj)
{
  return get(name, meta::typeof(obj), &obj);
}
template <class T>
const T& object_container::get(const string& name)
{
  return *reinterpret_cast<const T*>(get(name).data());
}
}