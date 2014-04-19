#pragma once

namespace meta {

template <class T>
void Serialize(serializer* s, const string& name, const T& object)
{
  typeof<T>()->Serialize(s, name, &object);
}
template <class T>
void Deserialize(serializer* s, const string& name, T& object)
{
  typeof<T>()->Deserialize(s, name, &object);
}

class constant
{
public:
  constant() = default;
  constant(const constant&) = default;

  template <class T>
  constant(const string& name_, const T& value_)
    : name(name_), value(value_), type(typeof<T>()) {}

  const string name = "uninitialized_constant";
  const variant value;
  const type* const type = typeof<nulltype>();
};

template <class T>
struct read_write
{
  static void read(serializer* s, const string& name, void* object) {}
  static void write(serializer* s, const string& name, const void* object) {}
};
#define META_SPECIALIZE_READ_WRITE(T, FUNCTION)                                       \
template <>                                                                           \
struct read_write<T>                                                                  \
{                                                                                     \
  static void read(serializer* s, const string& name, void* object)                   \
  {                                                                                   \
    *(T*)object = s->Read##FUNCTION(name);                                            \
  }                                                                                   \
  static void write(serializer* s, const string& name, const void* object)            \
  {                                                                                   \
    s->Write##FUNCTION(name, *(T*)object);                                            \
  }                                                                                   \
};

META_SPECIALIZE_READ_WRITE(bool, Bool)
META_SPECIALIZE_READ_WRITE(char, Integer)
META_SPECIALIZE_READ_WRITE(short, Integer)
META_SPECIALIZE_READ_WRITE(int, Integer)
META_SPECIALIZE_READ_WRITE(unsigned char, Integer)
META_SPECIALIZE_READ_WRITE(unsigned short, Integer)
META_SPECIALIZE_READ_WRITE(unsigned int, Integer)
META_SPECIALIZE_READ_WRITE(float, Float)
META_SPECIALIZE_READ_WRITE(double, Double)
META_SPECIALIZE_READ_WRITE(string, String)

template <class T>
struct enum_read_write
{
  static void read(serializer* s, const string& name, void* object)
  {
    //*(T*)object = s->ReadEnum(name).get_as<T>();
  }
  static void write(serializer* s, const string& name, const void* object)
  {
    //s->WriteEnum(name, *(T*)object);
  }
};

template <class T>
type::type(decl<T>, const string name, ReadFunc r, WriteFunc w, StringizeFunc str)
  : id(++S_ID_COUNTER)
  , size(sizeof(T))
  , name(name)
  , m_read(std::is_enum<T>::value ? &enum_read_write<T>::read : &read_write<T>::read)
  , m_write(std::is_enum<T>::value ? &enum_read_write<T>::write : &read_write<T>::write)
  , toString(str)
  , construct(std::has_default_constructor<T>::value ? &PlacementNew<std::conditional<std::has_default_constructor<T>::value, T, decl<T>>::type> : nullptr)
  , copy(std::is_copy_constructible<T>::value ? &CopyMemory<T> : nullptr)
  , destruct(&CallDestructor<T>)
  , clone(std::is_copy_constructible<T>::value ? &CloneObject<T> : nullptr)
  , allocate(&AllocMemory<T>)
  , deallocate(&DeleteMemory<T>)
  , assign((std::is_arithmetic<T>::value || std::is_enum<T>::value || std::is_assignable<T, T>::value) ? &AssignMemory<T> : nullptr)
  , isArithmetic(std::is_arithmetic<T>::value)
  , isPolymorphic(std::is_polymorphic<T>::value)
  , isIntegral(std::is_integral<T>::value)
  , isFloatingPoint(std::is_floating_point<T>::value)
  , isSigned(std::is_signed<T>::value)
  , isPod(std::is_pod<T>::value)
  , isEnum(std::is_enum<T>::value)
  //strings are considered basic types by my meta
  , isObject(std::is_class<T>::value && (std::is_same<T, string>::value == false))
{
  assert(construct != nullptr);
  assert(copy != nullptr);
  assert(destruct != nullptr);
  assert(clone != nullptr);
  assert(allocate != nullptr);
  assert(deallocate != nullptr);
  //assert assign is true if enum is false
  assert((isEnum == false) ? (assign != nullptr) : true);

  bool registered = internal::meta_registry::register_type(this);
  assert(registered == true);
}
inline void type::destroy()
{
  m_constants.clear();
  m_constantMap.clear();
  m_properties.clear();
  m_propertyMap.clear();
  m_fields.clear();
  m_fieldMap.clear();
  m_events.clear();
  m_eventMap.clear();
}
inline void type::Serialize(serializer* s, const string& name, const void* object) const
{
  if (isObject)
  {
    s->BeginObject(name, this);
    for (auto field : m_fields)
    {
      field.type->Serialize(s, field.name, field.member_ptr(object));
    }

    //for (auto property : m_properties)
    //{
    //  const variant value = property.get(object);
    //  property.type->Serialize(s, property.name, value.data());
    //}
    s->EndObject();
  }
  else
  {
    m_write(s, name, object);
  }
}
inline void type::Deserialize(serializer* s, const string& name, void* object) const
{
  if (isObject)
  {
    for (auto field : m_fields)
    {
      field.type->Deserialize(s, field.name, field.member_ptr(object));
    }

    for (auto property : m_properties)
    {
      variant value = property.get(object);
      property.type->Deserialize(s, property.name, value.data());
    }
  }
  else
  {
    m_read(s, name, object);
  }
}
inline bool type::operator==(const type& rhs) const
{
  return id == rhs.id;
}
inline bool type::operator!=(const type& rhs) const
{
  return !(*this == rhs);
}

inline bool type::isConvertible(const type* toType) const
{
  assert(toType);

  if (this == toType || baseType == toType)
    return true;
  
  if (isArithmetic && toType->isArithmetic)
    return true;

  return false;
}
template <class T>
bool type::isConvertible() const
{
  return isConvertible(typeof<T>());
}

namespace internal
{
  template <class T>
  void AddElementToNamedVector(const string& name, T&& element, hash_map<string, size_t>& map, vector<const T>& vec);
  template <class T>
  const T GetElementFromNamedVector(const string& name, const hash_map<string, size_t>& map, const vector<const T>& vec);
}

template <class T, class U>
type& type::Field(const string& name, T U::*var)
{
  assert(typeof<U>() == this);
  internal::AddElementToNamedVector(name, meta::field(name, typeof<T>(), ((size_t)&(((U*)0)->*var))), m_fieldMap, m_fields);
  return *this;
}

template <class T, class U>
type& type::Property(const string& name, T(U::*get)()const, void(U::*set)(T))
{
  internal::AddElementToNamedVector(name, meta::property(name, get, set), m_propertyMap, m_properties);
  return *this;
}
template <class T, class U>
type& type::Property(const string& name, void(U::*set)(T))
{
  internal::AddElementToNamedVector(name, meta::property(name, set), m_propertyMap, m_properties);
  return *this;
}
template <class T, class U>
type& type::Property(const string& name, T(U::*get)()const)
{
  internal::AddElementToNamedVector(name, meta::property(name, get), m_propertyMap, m_properties);
  return *this;
}

template <class U, class R, class... Args>
type& type::Event(const string& name, R(U::*func)(Args...))
{
  assert(typeof<U>() == this);
  internal::AddElementToNamedVector(name, meta::event(name, func), m_eventMap, m_events);
  return *this;
}
template <class U, class R, class... Args>
type& type::Event(const string& name, R(U::*func)(Args...)const)
{
  assert(typeof<U>() == this);
  internal::AddElementToNamedVector(name, meta::event(name, func), m_eventMap, m_events);
  return *this;
}

template <class T>
type& type::Constant(const string& name, const T& value)
{
  static_assert((std::is_pointer<T>::value == false), "Constant variable is invalid type (pointer type)");

  internal::AddElementToNamedVector(name, meta::constant(name, value), m_constantMap, m_constants);
  return *this;
}

inline const field type::getField(const string& name) const
{
  return internal::GetElementFromNamedVector(name, m_fieldMap, m_fields);
}
inline const property type::getProperty(const string& name) const
{
  return internal::GetElementFromNamedVector(name, m_propertyMap, m_properties);
}
inline const event type::getEvent(const string& name) const
{
  return internal::GetElementFromNamedVector(name, m_eventMap, m_events);
}
inline const constant type::getConstant(const string& name) const
{
  return internal::GetElementFromNamedVector(name, m_constantMap, m_constants);
}
inline const constant type::getConstant(int value) const
{
  for (auto constant : m_constants)
  {
    if (value == constant.value.get_as<int>())
      return constant;
  }
  return constant();
}

namespace internal {
template <class T>
void AddElementToNamedVector(const string& name, T&& element, hash_map<string, size_t>& map, vector<const T>& vec)
{
  assert(map.find(name) == map.end());
  map[name] = vec.size();
  vec.push_back(element);
}
template <class T>
const T GetElementFromNamedVector(const string& name, const hash_map<string, size_t>& map, const vector<const T>& vec)
{
  auto index = map.find(name);
  if (index == map.end())
    return T();

  return vec[index->second];
}
}}
