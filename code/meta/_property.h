#pragma once

namespace meta {
class property
{
public:
  property(const string& name, const function& get, const function& set);

  const string& name() const { return m_name; }
  const type* type() const { return m_type; }

  template <class U, class T>
  void set(U* object, const T& value) const;
  template <class U>
  variant get(U* object) const;
private:
  string m_name = "uninitialized_property";
  const ::meta::type* m_type;
  const ::meta::type* m_classType;
  const function m_get;
  const function m_set;
};
}