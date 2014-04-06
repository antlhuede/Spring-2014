#pragma once

namespace meta {
inline variant::variant() : m_type(&typeof<nulltype>()){}
inline variant::variant(variant&& other)
{
  m_type = other.m_type;

  if (other.m_data == other.m_array)
  {
    m_data = m_array;
    m_type->copy(m_data, other.m_data);
  }
  else
  {
    m_data = other.m_data;
    other.m_type = &typeof<nulltype>();
    other.m_data = nullptr;
  }
}
template <class T>
variant::variant(const T& obj)
{
  construct(typeof<T>(), &obj);
}
inline variant::variant(const variant& obj)
{
  construct(obj.type(), obj.data());
}
inline variant::variant(const meta::type& type, const void* obj)
{
  construct(type, obj);
}
inline variant::variant(const variant_ref& obj)
{
  construct(obj.type(), obj.data());
}
inline variant::variant(const char_t* str)
{
  string str_obj(str);
  construct(typeof<string>(), &str_obj);
}
inline variant::~variant()
{
  destroy();
}
inline void variant::destroy()
{
  if (m_data == nullptr)
    return;

  assert(m_type);

  m_type->destruct(m_data);
  
  if (m_data != m_array)
    m_type->deallocate(m_data);

  m_data = nullptr;

  m_type = &typeof<nulltype>();
}

inline variant& variant::operator=(const variant& rhs)
{
  if (this != &rhs)
  {
    destroy();
    construct(rhs.type(), rhs.data());
  }

  return *this;
}
inline void variant::construct(const meta::type& type, const void* memory)
{
  m_type = &type;
  
  //conditionals:
  //if type can fit in array, assign to array pointer and appropriately construct memory
  //otherwise, appropriately either new up or clone a pointer to the object
  if (type.size() <= SMALL_OBJECT_SIZE)
  {
    m_data = m_array;

    if (memory == nullptr)
      type.construct(m_data);
    else
      type.copy(m_data, memory);
  }
  else if (memory == nullptr)
  {
    m_data = type.allocate();
  }
  else
  {
    m_data = type.clone(memory);
  }
}
template <class T>
bool variant::is_type() const
{
  return typeof<T>() == *m_type;
}
template <class T>
bool variant::is_same_as(const T& obj) const
{
  return is_type<T>();
}
template <class T>
const T& variant::get_as() const
{
  return const_cast<variant*>(this)->get_as<T>();
}
template <class T>
T& variant::get_as()
{
  return converter::convert<T>(*m_type, m_data);
}

inline bool variant::can_convert(const meta::type& type) const
{
  return *m_type == type;
}
inline const string variant::to_string() const
{
  return m_type->to_string(m_data);
}

template <class T>
void variant::take_ownership(T* pData)
{
  destroy();

  m_type = &typeof<T>();
  m_data = pData;
}
inline void variant::take_ownership(variant_ref& data)
{
  assert(false);
}

template <class T> variant_ref::variant_ref(const T& obj)
  : m_data(&const_cast<T&>(obj))
  , m_type(&typeof<T>()) {}

inline variant_ref::variant_ref() : m_data(nullptr), m_type(&typeof<nulltype>()) {}
inline variant_ref::variant_ref(const variant& other) : m_data(other.m_data), m_type(other.m_type) {}
inline variant_ref::variant_ref(const variant_ref& other) : m_data(other.m_data), m_type(other.m_type) {}
inline variant_ref::variant_ref(variant_ref&& other) : m_data(other.m_data), m_type(other.m_type) {}

inline variant_ref& variant_ref::operator=(const variant& other)
{
  m_data = other.m_data;
  m_type = other.m_type;
  return *this;
}
inline variant_ref& variant_ref::operator=(const variant_ref& other)
{
  m_data = other.m_data;
  m_type = other.m_type;
  return *this;
}
}