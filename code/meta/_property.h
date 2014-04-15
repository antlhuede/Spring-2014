#pragma once

namespace meta {
class property
{
public:
  property() = default;
  template <class U, class T>
  property(const string& name, T(U::*get)()const, void(U::*set)(T));

  const string& name() const { return m_name; }
  const type* type() const { return m_type; }

  template <class U, class T>
  void set(U* object, const T& value) const;
  template <class U>
  variant get(U* object) const;
private:
  string m_name = "uninitialized_property";
  const ::meta::type* m_type = typeof<nulltype>();
  const ::meta::type* m_classType = typeof<nulltype>();
  const function m_get;
  const function m_set;
};
}