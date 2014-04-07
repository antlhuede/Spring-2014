#pragma once

#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <tuple>
#include <functional>
#include <iostream>
#include <sstream>
#include <cassert>

typedef char char_t;
template <class T> using vector = std::vector<T>;
template <class T> using shared_ptr = std::shared_ptr<T>;
template <class T, class U> using hash_map = std::unordered_map<T, U>;
typedef std::string string;
typedef std::istream istream;
typedef std::ostream ostream;
typedef std::ostringstream ostringstream;
typedef std::istringstream istringstream;

class nulltype {};
class void_ {};

namespace meta
{
  class type;
  class field;
  template <class T> struct decl {};

  typedef void(*ReadFunc)(istream&, void*);
  typedef void(*WriteFunc)(ostream&, void*);
  typedef void(*PlacementNewFunc)(void*);
  typedef void(*CopyFunc)(void* dest, const void* source);
  typedef void(*DestructFunc)(void*);
  typedef void(*DeleteFunc)(void*);
  typedef const string(*StringizeFunc)(const void*);
  typedef void*(*CloneFunc)(const void*);
  typedef void*(*AllocFunc)();
  typedef void(*AssignFunc)(void*, const void*);
}

typedef hash_map<string, const meta::type*> type_map;
