#pragma once

namespace meta {
class type : public list<type>
{
public:
  template <class T>
  explicit type(decl<T>, const string name, ReadFunc r, WriteFunc w, StringizeFunc str);
  ~type() = default;

  type(type&&) = delete;
  type(const type& rhs) = delete;
  type& operator=(const type& rhs) = delete;

  bool operator==(const type& rhs) const;
  bool operator!=(const type& rhs) const;
  
  const field getField(const string& name) const;
  const property getProperty(const string& name) const;
  const constant getConstant(const string& name) const;
  const event getEvent(const string& name) const;

  const vector<const meta::field>& fields() const { return m_fields; }
  const vector<const meta::property>& properties() const { return m_properties; }
  const vector<const meta::constant>& constants() const { return m_constants; }
  const vector<const meta::event>& events() const { return m_events; }

  template <class T>
  bool isConvertible() const;
  bool isConvertible(const type* toType) const;

  template <class T, class U>
  type& Field(const string& name, T U::*var);

  template <class T, class U>
  type& Property(const string& name, T(U::*get)()const, void(U::*set)(T));

  //functions to enable passing nullptr into either getter or setter without explicit
  //casting to the pointer type
  template <class T, class U>
  type& Property(const string& name, void(U::*set)(T));
  template <class T, class U>
  type& Property(const string& name, T(U::*get)()const);

  template <class U, class R, class... Args>
  type& Event(const string& name, R(U::*func)(Args...));
  template <class U, class R, class... Args>
  type& Event(const string& name, R(U::*func)(Args...)const);

  template <class T>
  type& Constant(const string& name, const T& value);

  //NEVER EVER EVER CALL THIS FUNCTION
  //it would require const_casting the type* and explicitly calling it
  //it is called during shutdown of meta
  void destroy();

  //const public member variables, better than private with getters
  const unsigned id = 0;
  const size_t size = 0;
  const string name = "nulltype";
  const type* baseType = nullptr;

  const ReadFunc read = nullptr;
  const WriteFunc write = nullptr;
  const StringizeFunc toString = nullptr;

  const PlacementNewFunc construct = nullptr;
  const CopyFunc copy = nullptr;
  const DestructFunc destruct = nullptr;
  const AllocFunc allocate = nullptr;
  const DeleteFunc deallocate = nullptr;
  const CloneFunc clone = nullptr;
  const AssignFunc assign = nullptr;
  
  const bool isEnum = false;
  const bool isObject = false;

  const bool isArithmetic = false;
  const bool isPolymorphic = false;
  const bool isIntegral = false;
  const bool isFloatingPoint = false;
  const bool isSigned = false;
  const bool isPod = false;

private:
  static unsigned S_ID_COUNTER;
  vector<const meta::field> m_fields;
  hash_map<string, size_t> m_fieldMap;

  vector<const meta::property> m_properties;
  hash_map<string, size_t> m_propertyMap;

  vector<const meta::event> m_events;
  hash_map<string, size_t> m_eventMap;

  vector<const meta::constant> m_constants;
  hash_map<string, size_t> m_constantMap;
};

}
