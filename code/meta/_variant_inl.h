#pragma once

namespace meta {
inline variant::variant() : m_type(&typeof<nulltype>()), m_data(nullptr) {}
//inline variant::variant(const ::meta::type& type)
//{
//  m_type = &type;
//  m_data = nullptr;
//}

template <class T>
variant::variant(const T& obj)
{
  copy_memory(typeof<T>(), &obj);
}
inline variant::variant(const variant& obj)
{
  copy_memory(obj.type(), obj.m_data);
}
inline variant::variant(const meta::type& type, const void* obj, bool take_ownership)
{
  if (take_ownership)
    this->take_ownership(type, const_cast<void*>(obj));
  else
    copy_memory(type, obj);
}
inline variant::variant(const variant_ref& obj)
{
  assert(false);
}
inline variant::variant(const char_t* str)
{
  string str_obj(str);
  copy_memory(typeof<string>(), &str_obj);
}
inline variant::~variant()
{
  destroy();
}
inline variant& variant::operator=(const variant& rhs)
{
  destroy();
  copy_memory(rhs.type(), rhs.m_data);

  return *this;
}
inline void variant::copy_memory(const meta::type& type, const void* memory)
{
  m_type = &type;

  if (type.size() <= SMALL_OBJECT_SIZE)
  {
    m_data = m_array;
    type.copy(m_data, memory);
  }
  else
    m_data = type.clone(memory);
}
inline void variant::destroy()
{
  if (m_data != m_array)
    m_type->delete_ptr(m_data);
  else
  {
    m_type->destruct(m_data);
    m_data = nullptr;
  }
  m_data = nullptr;
  m_type = &typeof<nulltype>();
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
T variant::get_as() const
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
  take_ownership(typeof<T>(), pData);
}
inline void variant::take_ownership(const meta::type& type, void* pData)
{
  m_type = &type;
  m_data = pData;
}
inline void variant::take_ownership(variant_ref& data)
{
  assert(false);
}
}