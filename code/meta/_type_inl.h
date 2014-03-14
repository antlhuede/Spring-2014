#pragma once

namespace meta
{
  template <class T>
  const type& typeof(const T& var) { return meta_holder<T>::t; }
  template <class T>
  const type& typeof() { return meta_holder<T>::t; }

  template <class T>
  void PlacementNew(void* memory)
  {
    new (memory) T();
  }
  template <class T>
  void CreateCopy(void* dest, void* source)
  {
    new (dest) T(*((T*)source));
  }
  template <class T>
  void MoveObject(void* dest, void* source)
  {
    new (dest) T(*((T*)source));
  }
  template <class T>
  void CallDestructor(void* object)
  {
    ((T*)object)->~T();
  }

  template <class T>
  type::type(decl<T>, const string name, ReadFunc r, WriteFunc w, StringizeFunc str)
    : m_id(++S_ID_COUNTER)
    , m_size(sizeof(T))
    , m_name(name)
    , read(r)
    , write(w)
    , to_string(str)
    , construct(std::has_default_constructor<T>::value ? &PlacementNew<std::conditional<std::has_default_constructor<T>::value, T, decl<T>>::type> : nullptr)
    , copy(std::has_copy_constructor<T>::value ? &CreateCopy<std::conditional<std::has_copy_constructor<T>::value, T, decl<T>>::type> : nullptr)
    , move(std::has_move_constructor<T>::value ? &MoveObject<std::conditional<std::has_move_constructor<T>::value, T, decl<T>>::type> : nullptr)
    , destruct(std::has_trivial_destructor<T>::value ? &CallDestructor<std::conditional<std::has_trivial_destructor<T>::value, T, decl<T>>::type> : nullptr) {}

  type& type::operator=(const type& rhs)
  {
    m_size = rhs.m_size;
    m_name = rhs.m_name;
    const_cast<ReadFunc&>(read) = rhs.read;
    const_cast<WriteFunc&>(write) = rhs.write;
    return *this;
  }

  bool type::operator==(const type& rhs) const
  {
    return m_id == rhs.m_id;
  }

  template <typename T, typename U>
  type& type::Field(const string& name, T U::*var)
  {
    m_fields.push_back(new field(name, meta_holder<T>::t, ((size_t)&(((U*)0)->*var))));
    return *this;
  }

  field::field(const string& name, const ::meta::type& type, size_t offset)
    : m_name(name), m_type(type), m_offset(offset) {}

  const string& field::name() const { return m_name; }
  const type& field::type() const { return m_type; }
  size_t field::offset() const { return m_offset; }

  void* field::member_ptr(void* object_ptr) const
  {
    return ((char*)object_ptr) + m_offset;
  }
  const string field::to_string(void* var) const
  {
    return m_type.to_string(member_ptr(var));
  }

  const type& meta_registry::find(const string& key)
  {
    auto it = m_registry.find(key);
    if (it == m_registry.end())
      return meta_holder<nulltype>::t;

    return *it->second;
  }

  bool meta_registry::register_type(const type* t)
  {
    if (m_registry.find(t->name()) != m_registry.end())
      return false;

    m_registry[t->name()] = t;
    return true;
  }

  template <class T>
  const string to_string_basic_types(void* var)
  {
    ostringstream buff;
    buff << *((T*)var);
    return buff.str();
  }

  template <class T>
  void read_basic_types(istream& stream, void* memory)
  {
    stream >> *reinterpret_cast<T*>(memory);
  }
  template <class T>
  void write_basic_types(ostream& stream, void* memory)
  {
    stream << *reinterpret_cast<T*>(memory);
  }

  template <class T>
  const string to_string_objects(void* memory)
  {
    return typeof<T>().name();
    /*const type& obj_type = typeof<T>();
    const vector<const field*>& field_list = obj_type.fields();
    ostringstream buff;
    buff << "\"" << obj_type.name() << "\"\n";
    for (size_t i = 0; i < field_list.size(); ++i)
    {
    const field& field = *field_list[i];
    const type& field_type = field.type();
    char* field_memory = ((char*)memory) + field.offset();
    buff << "\"" << field_type.name() << "\" : " << field_type.to_string(field_memory) << "\n";
    }
    return buff.str();*/
  }

#define ITERATE_OBJECTS(CALL_FUNC)                      \
  const type& obj_type = typeof<T>();                   \
  const vector<const field*>& field_list = obj_type.fields(); \
  for (size_t i = 0; i < field_list.size(); ++i)        \
  {                                                     \
  const field& field = *field_list[i];                \
  const type& field_type = field.type();              \
  char* field_memory = ((char*)memory) + field.offset();  \
  \
  field_type.CALL_FUNC(stream, field_memory);         \
  }

  template <class T>
  void read_objects(istream& stream, void* memory)
  {
    ITERATE_OBJECTS(read)
  }
  template <class T>
  void write_objects(ostream& stream, void* memory)
  {
    ITERATE_OBJECTS(write)
  }

  int converter::toInt(const type& t, void* obj)
  {
    return convert<int>(t, obj);
  }
  bool converter::toBool(const type& t, void* obj)
  {
    return convert<bool>(t, obj);
  }
  float converter::toFloat(const type& t, void* obj)
  {
    return convert<float>(t, obj);
  }
  double converter::toDouble(const type& t, void* obj)
  {
    return convert<double>(t, obj);
  }
  string converter::toString(const type& t, void* obj)
  {
    return convert<string>(t, obj);
  }

  template <typename T>
  static T converter::convert(const type& t, void* obj)
  {
    if (t == typeof<T>())
      return *((T*)obj);

    return T();
  }

  template <typename T>
  static bool converter::safe_convert(const type& t, void* obj, T* result)
  {
    if (t != typeof<T>())
      return false;

    *result = *((T*)obj);
    return true;
  }
}


#define DECLARE_META_TYPE(TYPE) \
  const meta::type meta::meta_holder<TYPE>::t(meta::decl<TYPE>(), #TYPE, &meta::read_basic_types<TYPE>, &meta::write_basic_types<TYPE>, &meta::to_string_basic_types<TYPE>);

#define DECLARE_META_OBJECT(OBJECT) \
  const meta::type meta::meta_holder<OBJECT>::t(meta::decl<OBJECT>(), #OBJECT, &meta::read_objects<OBJECT>, &meta::write_objects<OBJECT>, &meta::to_string_objects<OBJECT>);
