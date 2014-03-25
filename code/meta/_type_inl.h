#pragma once

namespace meta
{

template <class T>
type::type(decl<T>, const string name, ReadFunc r, WriteFunc w, StringizeFunc str)
  : m_id(++S_ID_COUNTER)
  , m_size(sizeof(T))
  , m_name(name)
  , read(r)
  , write(w)
  , to_string(str)
  , construct(std::has_default_constructor<T>::value ? &PlacementNew<std::conditional<std::has_default_constructor<T>::value, T, decl<T>>::type> : nullptr)
  , copy(std::has_copy_constructor<T>::value ? &CopyMemory<std::conditional<std::has_copy_constructor<T>::value, T, decl<T>>::type> : nullptr)
  , destruct(&CallDestructor<T>)
  , clone(&CloneObject<T>)
  , make_new(&AllocMemory<T>)
  , delete_ptr(&DeletePointer<T>) { assert(internal::meta_registry::register_type(this) == true); }

inline type& type::operator=(const type& rhs)
{
  m_id = rhs.m_id;
  m_size = rhs.m_size;
  m_name = rhs.m_name;
  const_cast<ReadFunc&>(read) = rhs.read;
  const_cast<WriteFunc&>(write) = rhs.write;
  const_cast<StringizeFunc&>(to_string) = rhs.to_string;
  const_cast<PlacementNewFunc&>(construct) = rhs.construct;
  const_cast<CopyFunc&>(copy) = rhs.copy;
  const_cast<DestructFunc&>(destruct) = rhs.destruct;
  const_cast<CloneFunc&>(clone) = rhs.clone;
  const_cast<AllocFunc&>(make_new) = rhs.make_new;

  m_fields = rhs.m_fields;
  m_fieldmap = rhs.m_fieldmap;

  return *this;
}

inline bool type::operator==(const type& rhs) const
{
  return m_id == rhs.m_id;
}
inline bool type::operator!=(const type& rhs) const
{
  return !(*this == rhs);
}

template <typename T, typename U>
type& type::Field(const string& name, T U::*var)
{
  assert(m_fieldmap.find(name) == m_fieldmap.end());
  m_fieldmap[name] = m_fields.size();
  m_fields.push_back(shared_ptr<const ::meta::field>(new ::meta::field(name, typeof<T>(), ((size_t)&(((U*)0)->*var)))));
  return *this;
}

inline const field type::field(const string& name, bool assert_on_failure) const
{
  auto index = m_fieldmap.find(name);
  if (index == m_fieldmap.end() && (assert_on_failure ? assert(false), false : true))
    return ::meta::field("empty_field", typeof<nulltype>(), 0);
    
  return *m_fields[index->second];
}

}
