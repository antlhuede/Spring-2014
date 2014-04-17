#pragma once

namespace meta {
namespace internal
{
  template <class T, class Map, class Vector>
  void AddElementToNamedVector(const string& name, T&& element, Map& map, Vector& vec);
  template <class T, class Map, class Vector>
  const T GetElementFromNamedVector(const string& name, const Map& map, const Vector& vec);
}
class constant
{
public:
  constant() = default;
  constant(const constant&) = default;

  string name = "uninitialized_constant";
  const variant value;
};

template <class T>
type::type(decl<T>, const string name, ReadFunc r, WriteFunc w, StringizeFunc str)
  : m_id(++S_ID_COUNTER)
  , m_size(sizeof(T))
  , m_name(name)
  , m_read(r)
  , m_write(w)
  , m_to_string(str)
  , m_construct(std::has_default_constructor<T>::value ? &PlacementNew<std::conditional<std::has_default_constructor<T>::value, T, decl<T>>::type> : nullptr)
  , m_copy(std::has_copy_constructor<T>::value ? &CopyMemory<std::conditional<std::has_copy_constructor<T>::value, T, decl<T>>::type> : nullptr)
  , m_destruct(&CallDestructor<T>)
  , m_clone(&CloneObject<T>)
  , m_allocate(&AllocMemory<T>)
  , m_deallocate(&DeleteMemory<T>)
  , m_assign(&AssignMemory<T>) 
{
  bool registered = internal::meta_registry::register_type(this);
  assert(registered == true);
}

inline void type::read(istream& stream, void* memory) const   
{
  assert(m_read);
  m_read(stream, memory);
}
inline void type::write(ostream& stream, void* memory) const  
{
  assert(m_write);
  m_write(stream, memory);
}
inline void type::construct(void* memory) const               
{
  assert(m_construct);
  m_construct(memory);
}
inline void type::destruct(void* memory) const                
{
  assert(m_destruct);
  m_destruct(memory);
}
inline void type::copy(void* dest, const void* source) const  
{
  assert(m_copy);
  m_copy(dest, source);
}
inline const string type::to_string(const void* memory) const 
{
  assert(m_to_string);
  return m_to_string(memory);
}
inline void* type::clone(const void* source) const            
{
  assert(m_clone);
  return m_clone(source);
}
inline void* type::allocate() const                           
{
  assert(m_allocate);
  return m_allocate();
}
inline void type::deallocate(void* memory) const              
{
  assert(m_deallocate);
  m_deallocate(memory);
}
inline void type::assign(void* dest, const void* source) const
{
  assert(m_assign);
  m_assign(dest, source);
}
inline bool type::operator==(const type& rhs) const
{
  return m_id == rhs.m_id;
}
inline bool type::operator!=(const type& rhs) const
{
  return !(*this == rhs);
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

template <class T>
type& type::Constant(const string& name, const T& value)
{
  
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

inline const field type::field(const string& name) const
{
  return internal::GetElementFromNamedVector<meta::field>(name, m_fieldMap, m_fields);
}
inline const property type::property(const string& name) const
{
  return internal::GetElementFromNamedVector<meta::property>(name, m_propertyMap, m_properties);
}
inline const event type::event(const string& name) const
{
  return internal::GetElementFromNamedVector<meta::event>(name, m_eventMap, m_events);
}

namespace internal {
template <class T, class Map, class Vector>
void AddElementToNamedVector(const string& name, T&& element, Map& map, Vector& vec)
{
  assert(map.find(name) == map.end());
  map[name] = vec.size();
  vec.push_back(element);
}
template <class T, class Map, class Vector>
const T GetElementFromNamedVector(const string& name, const Map& map, const Vector& vec)
{
  auto index = map.find(name);
  if (index == map.end())
    return T();

  return vec[index->second];
}
}}
