#pragma once

//basic type definitions
typedef char char_t;
typedef unsigned char uchar_t;

typedef signed __int8 int8;
typedef signed __int16 int16;
typedef signed __int32 int32;
typedef signed __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

typedef float f32;
typedef double f64;

typedef uint64 tag;
typedef uint32 slot_index;
typedef uint32 object_id;

//fnv hash types
typedef uint32 Fnv32_t;
typedef uint64 Fnv64_t;

//constexpr functions to get the index or id from a tag, and to build a tag from index/id
//make slot 64bit by or'ing it with a zero uint64, then shift left 32bits, then ensure bottom bits are cancelled out and preserve top bits before oring with the id ensuring negated top bits
inline tag make_tag(slot_index slot, object_id object) { return ((uint64(slot) << 32) & 0xFFFFFFFF00000000) | object; }
inline slot_index get_index(tag t) { return slot_index((t >> 32) & 0x00000000FFFFFFFF); }
inline object_id get_id(tag t) { return object_id(t & 0x00000000FFFFFFFF); }

//hash function definition
#include "core\hashing\hash_functions.h"
typedef alib::core::hash_functions::FNV64 hash_function;
typedef hash_function::hash_type string_hash;

template <class T>
struct array_deleter { void operator()(void* p) { delete [] reinterpret_cast<T*>(p); } };

//forward declarations
namespace alib { 
  namespace containers { 
    class string;
    template <class T> class vector;
  } 
}
//container definitions
typedef alib::containers::string string;
template <class T> using vector = alib::containers::vector<T>;

#include <memory>
template <class T> using shared_ptr = std::shared_ptr<T>;
template <class T> using weak_ptr = std::weak_ptr<T>;

template <class T> using object = shared_ptr<T>;
template <class T> using object_ref = weak_ptr<T>;

#include <unordered_map>
template <class Key, class Value> using hash_map = std::unordered_map<Key, Value>;

#include <initializer_list>
template <class T> using initializer_list = std::initializer_list<T>;