#pragma once

namespace meta { namespace internal {
class type_initializer : public list<type_initializer>
{
public:
  void(*create_func)() = nullptr;
  void(*destroy_func)() = nullptr;
  void(*init_func)() = nullptr;

  type_initializer(void(*create)(), void(*destroy)(), void(*init)())
    : create_func(create)
    , destroy_func(destroy)
    , init_func(init) {}
};
}}