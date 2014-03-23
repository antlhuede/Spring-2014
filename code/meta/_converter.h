#pragma once

namespace meta
{

struct converter
{
  static int toInt(const type& t, const void* obj);
  static bool toBool(const type& t, const void* obj);
  static float toFloat(const type& t, const void* obj);
  static double toDouble(const type& t, const void* obj);
  static string toString(const type& t, const void* obj);

  template <typename T>
  static T convert(const type& t, const void* obj);

  template <typename T>
  static bool safe_convert(const type& t, const void* obj, T* result);
};

template <class T> const string to_string_basic_types(const void* var);
template <class T> const string to_string_objects(const void* memory);

template <class T> void read_basic_types(istream& stream, void* memory);
template <class T> void read_objects(istream& stream, void* memory);

template <class T> void write_basic_types(ostream& stream, const void* memory);
template <class T> void write_objects(ostream& stream, const void* memory);

}