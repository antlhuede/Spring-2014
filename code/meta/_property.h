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
  property(const string& name, T(U::*get)()const);
  template <class T, class U>
  property(const string& name, void(U::*set)(T));

  //template <class U>
  //variant get(const U* object) const;
  variant get(const void* object) const;
  template <class T, class U>
  T get_as(const U* object) const;

  template <class T, class U>
  void set(U* object, const T& value) const;
  template <class T>
  void set(void* object, const T& value) const;
  void set(const type* classType, void* object, const type* propertyType, const void* propertyValue);

  const function& getter() const { return m_get; }
  const function& setter() const { return m_set; }

  const bool hasSetter = false;
  const bool hasGetter = false;
  const string name = "uninitialized_property";
  const ::meta::type* const type = typeof<nulltype>();
  const ::meta::type* const classType = typeof<nulltype>();

private:
  const function m_get;
  const function m_set;
};
}