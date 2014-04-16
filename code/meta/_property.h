#pragma once

namespace meta {
class property
{
public:
  property() = default;
  ~property() = default;
  template <class T, class U>
  property(const string& name, T(U::*get)()const, void(U::*set)(T));

  template <class T, class U>
  void set(U* object, const T& value) const;
  template <class U>
  variant get(U* object) const;

  //less 'safe' since not guarenteeing object is correct type
  template <class T>
  void set(void* object, const T& value) const;
  variant get(const void* object) const;

  template <class T, class U>
  T get_as(U* object) const;

  bool has_getter() const { return m_hasGetter; }
  bool has_setter() const { return m_hasSetter; }

  const string& name() const { return m_name; }
  const type* type() const { return m_type; }
  const function& getter() const { return m_get; }
  const function& setter() const { return m_set; }
  
private:
  string m_name = "uninitialized_property";
  const ::meta::type* m_type = typeof<nulltype>();
  const ::meta::type* m_classType = typeof<nulltype>();
  const function m_get;
  const function m_set;
  bool m_hasSetter = false;
  bool m_hasGetter = false;
};
}