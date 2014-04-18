#include "meta\meta.h"

DECLARE_META_TYPE(char)
DECLARE_META_TYPE(unsigned char)
DECLARE_META_TYPE(int)
DECLARE_META_TYPE(unsigned int)
DECLARE_META_TYPE(short)
DECLARE_META_TYPE(unsigned short)
DECLARE_META_TYPE(bool)
DECLARE_META_TYPE(float)
DECLARE_META_TYPE(double)
DECLARE_META_TYPE(string)
DECLARE_META_OBJECT(void_)
END_META_OBJECT()
DECLARE_META_OBJECT(nulltype)
END_META_OBJECT()

namespace meta
{
unsigned type::S_ID_COUNTER = 0;
type_map* internal::meta_registry::m_registry = nullptr;

void initialize()
{
  internal::meta_registry::m_registry = new type_map;

  internal::type_initializer* it = internal::type_initializer::head();
  while (it != nullptr)
  {
    assert(it->create_func);
    assert(it->destroy_func);
    it->create_func();
    it = it->next();
  }

  it = internal::type_initializer::head();
  while (it != nullptr)
  {
    if (it->init_func)
      it->init_func();
    it = it->next();
  }
}
  
void destroy()
{
  internal::type_initializer* it = internal::type_initializer::head();
  //must shutdown first since some components
  //depend on type* being valid during destruction
  while (it != nullptr)
  {
    if (it->shutdown_func)
      it->shutdown_func();

    it = it->next();
  }
  it = internal::type_initializer::head();
  while (it != nullptr)
  {
    it->destroy_func();
    it = it->next();
  }

  delete internal::meta_registry::m_registry;
  internal::meta_registry::m_registry = nullptr;
}
}