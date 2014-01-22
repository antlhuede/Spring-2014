#include "core\types.h"
#include "core\hashing\hash_functions.h"
#include "core\hashing\fnv.h"
#include <iostream>

namespace alib 
{
namespace core
{
namespace hash_functions
{
  auto x65599::generate_hash(const char_t* str)-> hash_type
  {
    hash_type hash = do_hash(str, 0);
    return hash ^ (hash >> 16);
  }
  auto x65599::do_hash(const char_t* str, uint32 hash)-> hash_type
  {
    return (*str == 0) ? hash : do_hash(str + 1, *str + hash * 65599);
  }
  auto FNV32::generate_hash(const char_t* str)-> hash_type
  {
    return fnv_32a_str(str);
  }
  auto FNV64::generate_hash(const char_t* str)-> uint64
  {
    return fnv_64a_str(str);
  }
  auto sum::generate_hash(const char_t* str)-> hash_type
  {
    return (*str == 0) ? 0 : (*str + generate_hash(str + 1));
  }
}
}
}