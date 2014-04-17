#pragma once

namespace meta {
class event
{
public:
  event() = default;
  template <class U, class R, class... Args>
  event(const string& name_, R(U::*func)(Args...))
    : name(name), type(typeof<U>()), function(func) {}

  const string name = "uninitialized_property";
  const meta::type* const type = typeof<nulltype>();
  const meta::function function;

  template <class U, class... Args>
  variant trigger(U* obj, Args... args) const;
  template <class U, class... Args>
  variant trigger(const U* obj, Args... args) const;
};
}