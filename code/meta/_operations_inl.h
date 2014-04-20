#pragma once

namespace meta
{
template <class T> const type* typeof(const T& var) { return internal::meta_holder<std::remove_cv<std::remove_pointer<std::remove_reference<T>::type>::type>::type>::s_pType; }
template <class T> const type* typeof() { return internal::meta_holder<std::remove_cv<std::remove_pointer<std::remove_reference<T>::type>::type>::type>::s_pType; }
inline const type* typeof(const char_t* name) { return internal::meta_registry::find(name); }

template <class T> void PlacementNew(void* memory)
{
  new (memory) T();
}
template <class T> void CopyMemory(void* dest, const void* source)
{
  new (dest) T(*static_cast<const T*>(source));
}
template <class T> void CallDestructor(void* object)
{
  static_cast<T*>(object)->~T();
}
template <class T> void DeletePointer(void* object)
{
  delete static_cast<T*>(object);
}
template <class T> void* CloneObject(const void* object)
{
  return new T(*static_cast<const T*>(object));
}
template <class T> void* AllocMemory()
{
  return new T;
}
template <class T> void DeleteMemory(void* memory)
{
  delete static_cast<T*>(memory);
}
template <class T> void AssignMemory(void* dest, const void* source)
{
  *static_cast<T*>(dest) = *static_cast<const T*>(source);
}
template <class T> T& ConvertMemory(void* memory)
{
  return *static_cast<T*>(memory);
}

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
    const string title = s->ReadEnum(name);
    const type* type = typeof<T>();
    const variant& value = type->getConstant(title).value;
    *(T*)object = value.get_as<T>();
  }
  static void write(serializer* s, const string& name, const void* object)
  {
    const type* type = typeof<T>();
    for (auto constant : type->constants())
    {
      assert(constant.type == type);

      if (constant.value.get_as<T>() == *(T*)object)
      {
        s->WriteEnum(name, type, constant.name);
        return;
      }
    }
  }
};

template <class T, bool isEnum> struct read_write_selector;
template <class T> struct read_write_selector<T, false> : public read_write<T>{};
template <class T> struct read_write_selector<T, true> : public enum_read_write<T>{};

template <class T> struct to_string
{
  static const string toString(const void*) { return string(); }
};

#define META_SPECIALIZE_TO_STRING(T)                  \
template <> struct to_string<T>                       \
{                                                     \
  static const string toString(const void* object)    \
  {                                                   \
    ostringstream buff;                               \
    buff << *((T*)object);                            \
    return buff.str();                                \
  }                                                   \
};

META_SPECIALIZE_TO_STRING(bool)
META_SPECIALIZE_TO_STRING(char)
META_SPECIALIZE_TO_STRING(short)
META_SPECIALIZE_TO_STRING(int)
META_SPECIALIZE_TO_STRING(unsigned char)
META_SPECIALIZE_TO_STRING(unsigned short)
META_SPECIALIZE_TO_STRING(unsigned int)
META_SPECIALIZE_TO_STRING(float)
META_SPECIALIZE_TO_STRING(double)
META_SPECIALIZE_TO_STRING(string)

template <class T> struct enum_to_string
{
  static const string toString(const void* object)
  {
    const type* type = typeof<T>();
    for (auto constant : type->constants())
    {
      assert(constant.type == type);

      if (constant.value.get_as<T>() == *(T*)object)
      {
        return constant.name;
      }
    }
    return string();
  }
};
template <class T, bool isEnum> struct to_string_selector;
template <class T> struct to_string_selector<T, false> : public to_string<T>{};
template <class T> struct to_string_selector<T, true> : public enum_to_string<T>{};

}