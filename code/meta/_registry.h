#pragma once

namespace meta
{
template <class T>
struct meta_holder { static const type t; };

struct meta_registry
{
public:
  static const type& find(const string& key);

  static bool register_type(const type* t);
private:
  static hash_map<string, const type*> m_registry;
};

}