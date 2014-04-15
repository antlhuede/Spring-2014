#pragma once

namespace meta
{
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
inline const vector<const field>& type::fields() const
{
  return m_fields;
}

template <class T, class U>
type& type::Field(const string& name, T U::*var)
{
  assert(typeof<U>() == this);
  assert(m_fieldMap.find(name) == m_fieldMap.end());
  m_fieldMap[name] = m_fields.size();
  m_fields.push_back(::meta::field(name, typeof<T>(), ((size_t)&(((U*)0)->*var))));
  return *this;
}

template <class T, class U>
type& type::Property(const string& name, T(U::*get)()const, void(U::*set)(T))
{
  assert(m_propertyMap.find(name) == m_propertyMap.end());
  m_propertyMap[name] = m_properties.size();
  m_properties.push_back(::meta::property(name, get, set));
  return *this;
}
template <class T, class U>
type& type::Property(const string& name, void(U::*set)(T))
{
  typedef T(U::*getter_type)()const;
  return Property(name, (getter_type)nullptr, set);
}
template <class T, class U>
type& type::Property(const string& name, T(U::*get)()const)
{
  typedef void(U::*setter_type)(T);
  return Property(name, get, (setter_type)nullptr);
}


#define IMPLEMENT_EVENT_CONSTRUCTOR()                 \
  assert(typeof<U>() == this);                        \
  assert(m_eventMap.find(name) == m_eventMap.end());  \
  m_eventMap[name] = m_events.size();                 \
  m_events.push_back(::meta::event(name, func));      \
  return *this

template <class U, class R, class... Args>
type& type::Event(const string& name, R(U::*func)(Args...))
{
  IMPLEMENT_EVENT_CONSTRUCTOR();
}
template <class U, class R, class... Args>
type& type::Event(const string& name, R(U::*func)(Args...)const)
{
  IMPLEMENT_EVENT_CONSTRUCTOR();
}
#undef IMPLEMENT_EVENT_CONSTRUCTOR


#define IMPLEMENT_META_GET_FUNCTION(data_type, name_map, object_array)                \
  auto index = name_map.find(name);                                                   \
  if (index == name_map.end() && (assert_on_failure ? assert(false), true : true))    \
  return ::meta::data_type();                                                         \
  return object_array[index->second]

inline const field type::field(const string& name, bool assert_on_failure) const
{
  IMPLEMENT_META_GET_FUNCTION(field, m_fieldMap, m_fields);
}
inline const property type::property(const string& name, bool assert_on_failure) const
{
  IMPLEMENT_META_GET_FUNCTION(property, m_propertyMap, m_properties);
}
inline const event type::event(const string& name, bool assert_on_failure) const
{
  IMPLEMENT_META_GET_FUNCTION(event, m_eventMap, m_events);
}
#undef IMPLEMENT_META_GET_FUNCTION

}
