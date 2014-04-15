#pragma once

namespace meta {
class event
{
public:
  event() = default;
  template <class U, class R, class... Args>
  event(const string& name, R(U::*func)(Args...))
    : m_name(name), m_type(typeof<U>()), m_function(func) {}

  const string& name() const { return m_name; }
  const type* type() const { return m_type; }

  const function& function() const { return m_function; }

  template <class U, class... Args>
  variant trigger(U* obj, Args... args) const;
  template <class U, class... Args>
  variant trigger(const U* obj, Args... args) const;
private:
  string m_name = "uninitialized_property";
  const ::meta::type* m_type = typeof<nulltype>();
  const ::meta::function m_function;
};
}