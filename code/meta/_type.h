#pragma once

namespace meta
{
  class type;
  class field;
  class variant {};
  
  typedef void(*ReadFunc)(std::istream&, void*);
  typedef void(*WriteFunc)(std::ostream&, void*);
  typedef const string(*StringizeFunc)(void*);

  class type
  {
  public:
    type(size_t size, string name, ReadFunc r, WriteFunc w, StringizeFunc str) : m_id(++S_ID_COUNTER), m_size(size), m_name(name), read(r), write(w), to_string(str) {}
    type& operator=(const type& rhs)
    {
      m_size = rhs.m_size;
      m_name = rhs.m_name;
      const_cast<ReadFunc&>(read) = rhs.read;
      const_cast<WriteFunc&>(write) = rhs.write;
      return *this;
    }

    bool operator==(const type& rhs) const
    {
      return m_id == rhs.m_id;
    }

    size_t size() const { return m_size; }
    const string& name() const { return m_name; }
    unsigned id() const { return m_id; }

    const ReadFunc read;
    const WriteFunc write;
    const StringizeFunc to_string;

    template <typename T, typename U >
    type& Field(const string& name, T U::*var);

    const vector<const field*>& fields() const { return m_fields; }

  private:
    static unsigned S_ID_COUNTER;
    unsigned m_id = 0;
    size_t m_size = 0;
    string m_name = "uninitialized_type";
    vector<const field*> m_fields;
  };

  class field
  {
  public:
    field(const string& name, const type& type, size_t offset)
      : m_name(name), m_type(type), m_offset(offset) {}

    const string& name() const { return m_name; }
    const type& type() const { return m_type; }
    size_t offset() const { return m_offset; }

    void* member_ptr(void* object_ptr) const
    {
      return ((char*)object_ptr) + m_offset;
    }
    const string to_string(void* var) const
    {
      return m_type.to_string(member_ptr(var));
    }

  private:
    string m_name;
    ::meta::type m_type;
    size_t m_offset;
  };

  template <class T>
  struct meta_holder { static type t; };
    
  template <typename T, typename U>
  type& type::Field(const string& name, T U::*var)
  {
    m_fields.push_back(new field(name, meta_holder<T>::t, ((size_t)&(((U*)0)->*var))));
    return *this;
  }

  template <class T>
  const type& typeof(const T& var) { return meta_holder<T>::t; }
  template <class T>
  const type& typeof() { return meta_holder<T>::t; }

  template <class T>
  const string to_string_basic_types(void* var)
  {
    std::ostringstream buff;
    buff << *((T*)var);
    return buff.str();
  }

  template <class T>
  void read_basic_types(std::istream& stream, void* memory)
  {
    stream >> *reinterpret_cast<T*>(memory);
  }
  template <class T>
  void write_basic_types(std::ostream& stream, void* memory)
  {
    stream << *reinterpret_cast<T*>(memory);
  }

  
  template <class T>
  const string to_string_objects(void* memory)
  {
    return typeof<T>().name();
      /*const type& obj_type = typeof<T>();
      const vector<const field*>& field_list = obj_type.fields();
      std::ostringstream buff;
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
  void read_objects(std::istream& stream, void* memory)
  {
    ITERATE_OBJECTS(read)
  }
  template <class T>
  void write_objects(std::ostream& stream, void* memory)
  {
    ITERATE_OBJECTS(write)
  }

  struct converter
  {
    static int toInt(const type& t, void* obj)
    {
      return convert<int>(t, obj);
    }
    static bool toBool(const type& t, void* obj)
    {
      return convert<bool>(t, obj);
    }
    static float toFloat(const type& t, void* obj)
    {
      return convert<float>(t, obj);
    }
    static double toDouble(const type& t, void* obj)
    {
      return convert<double>(t, obj);
    }
    static string toString(const type& t, void* obj)
    {
      return convert<string>(t, obj);
    }

    template <typename T>
    static T convert(const type& t, void* obj)
    {
      if (t == typeof<T>())
        return *((T*)obj);

      return T();
    }

    template <typename T>
    static bool safe_convert(const type& t, void* obj, T* result)
    {
      if (t != typeof<T>())
        return false;

      *result = *((T*)obj);
      return true;
    }
  };
}

#define DECLARE_META_TYPE(TYPE) \
  meta::type meta::meta_holder<TYPE>::t(sizeof(TYPE), #TYPE, &meta::read_basic_types<TYPE>, &meta::write_basic_types<TYPE>, &meta::to_string_basic_types<TYPE>)


#define DECLARE_META_OBJECT(OBJECT) \
  meta::type meta::meta_holder<OBJECT>::t(sizeof(OBJECT), #OBJECT, &meta::read_objects<OBJECT>, &meta::write_objects<OBJECT>, &meta::to_string_objects<OBJECT>)
