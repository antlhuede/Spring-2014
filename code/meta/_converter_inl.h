#pragma once

namespace meta {

template <typename T>
static T& converter::convert(const type* t, const void* obj)
{
  return *((T*)obj);
}

template <typename T>
static bool converter::safe_convert(const type* t, const void* obj, T* result)
{
  if (*t != *typeof<T>())
    return false;

  *result = *((T*)obj);
  return true;
}

template <class T>
const string to_string_basic_types(const void* var)
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
const string to_string_objects(const void* memory)
{
  return typeof<T>()->name();
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
const type* obj_type = typeof<T>();                   \
const vector<const field>& field_list = obj_type->fields(); \
for (size_t i = 0; i < field_list.size(); ++i)        \
{                                                     \
const field& field = field_list[i];                \
const type* field_type = field.type();              \
field_type->CALL_FUNC(stream, field.member_ptr(memory));         \
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
#undef ITERATE_OBJECTS

inline int converter::toInt(const type* t, const void* obj)
{
  return convert<int>(t, obj);
}
inline bool converter::toBool(const type* t, const void* obj)
{
  return convert<bool>(t, obj);
}
inline float converter::toFloat(const type* t, const void* obj)
{
  return convert<float>(t, obj);
}
inline double converter::toDouble(const type* t, const void* obj)
{
  return convert<double>(t, obj);
}
inline string converter::toString(const type* t, const void* obj)
{
  return convert<string>(t, obj);
}

}