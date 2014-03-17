#pragma once

namespace meta
{

struct converter
{
  static int toInt(const type& t, void* obj);
  static bool toBool(const type& t, void* obj);
  static float toFloat(const type& t, void* obj);
  static double toDouble(const type& t, void* obj);
  static string toString(const type& t, void* obj);

  template <typename T>
  static T convert(const type& t, void* obj);

  template <typename T>
  static bool safe_convert(const type& t, void* obj, T* result);
};

template <class T> const string to_string_basic_types(void* var);
template <class T> const string to_string_objects(void* memory);

template <class T> void read_basic_types(istream& stream, void* memory);
template <class T> void read_objects(istream& stream, void* memory);

template <class T> void write_basic_types(ostream& stream, void* memory);
template <class T> void write_objects(ostream& stream, void* memory);

}