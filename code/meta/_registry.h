#pragma once

namespace meta
{
namespace internal {

template <class T>
struct meta_holder { static const type* s_pType; };

struct meta_registry
{
public:
  static const type* find(const string& key);

  static bool register_type(const type* pType);
private:
  static type_map* m_registry;
  friend void ::meta::initialize();
  friend void ::meta::destroy();
};

}
}