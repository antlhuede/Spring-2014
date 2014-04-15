#pragma once

namespace meta {
class event
{
public:
  template <class U, class R, class... Args>
  event(const string& name, R(U::*func)(Args...))
    : m_name(name), m_type(typeof<U>()), m_function(func) {}

  const string& name() const { return m_name; }
  const type* type() const { return m_type; }

  const function& function() const { return m_function; }

  template <class U, class... Args>
  void trigger(U* obj, Args&&... args)
  {
    bool correct_types = m_function.check_types<U, Args...>();
    assert(correct_types == true);

    m_function.call(obj, std::forward<Args&&>(args));
  }
  template <class U, class... Args>
  void trigger(const U* obj, Args&&... args) const
  {
    bool correct_types = m_function.check_types<U, Args...>();
    assert(correct_types == true);

    assert(m_function.traits().isConst == true);
    m_function.call(obj, std::forward<Args&&>(args));
  }
private:
  string m_name = "uninitialized_property";
  const ::meta::type* m_type;
  const ::meta::function m_function;
};
}