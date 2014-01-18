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
  auto x65599::generate_hash(const char* str)-> string_hash
  {
    string_hash hash = do_hash(str, 0);
    return hash ^ (hash >> 16);
  }
  auto x65599::do_hash(const char* str, string_hash hash)-> string_hash
  {
    return (*str == 0) ? hash : do_hash(str + 1, *str + hash * 65599);
  }

  auto FNV::generate_hash(const char* str)-> string_hash
  {
    return fnv_32a_str(str);
  }

  auto sum::generate_hash(const char* str)-> string_hash
  {
    return (*str == 0) ? 0 : (*str + generate_hash(str + 1));
  }
}
}
}