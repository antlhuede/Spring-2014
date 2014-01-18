#include "core\types.h"
#include "core\hashing\hash_functions.h"
#include "containers\string.h"
#include <cstring>
#include <cassert>


namespace alib
{
namespace containers
{
string_pool string::registry;

void string::register_string(const char* value, size length, string_hash hash)
{
  auto current_value = registry.find(hash);
  if (current_value == registry.end()) //no value exists yet, create a copy
  {
    char* copy = new char[length + 1];
    std::strcpy(copy, value);
    registry[hash] = copy;
  }

#ifdef STRING_HASH_DEBUGGING
  else //then a value for that string exists
  {
    //compare strings, error if different (2 strings hash to same value)
    assert(std::strcmp(current_value->second, value) == 0);
  }
#endif
}

string::string()
  : m_length(0)
  , m_hash(hash_function::generate_hash(""))
{
  register_string("", m_length, m_hash);
}
string::string(const char* value)
  : m_length(std::strlen(value))
  , m_hash(hash_function::generate_hash(value)) 
{
  register_string(value, m_length, m_hash);
}
string::string(const string& rhs)
  : m_length(rhs.m_length)
  , m_hash(rhs.m_hash) {}
string::string(string&& rhs)
  : m_length(rhs.m_length)
  , m_hash(rhs.m_hash) {}

string& string::operator=(const string& rhs)
{
  return copy_string(rhs);
}
string& string::operator=(string&& rhs)
{
  return copy_string(rhs);
}
bool string::operator==(const string& rhs) const
{
  return m_hash == rhs.m_hash;
}

char& string::operator[](size index) { return registry[m_hash][index]; }
const char& string::operator[](size index) const { return const_cast<string&>(*this)[index]; }

string_hash string::hash() const { return m_hash; }
size string::length() const { return m_length; }
const char* string::c_str() const
{
  auto value = registry.find(m_hash);
  assert(value != registry.end());
  return value->second;
}
}
}