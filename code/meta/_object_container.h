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

protected:
  hash_map<string, size_t> m_names;
  vector<variant> m_objects;
};

}