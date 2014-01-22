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

//helper funcs
shared_ptr<char_t> allocate_string(size_t length)
{
  return shared_ptr<char_t>(new char_t[length + 1], array_deleter<char_t>());
}
void strcat(string_literal* dest, const string_literal& append)
{
  size_t new_length = dest->m_strlen + append.m_strlen;
  if (dest->m_allocated < new_length)
  {
    shared_ptr<char_t> new_string = allocate_string(new_length);
    std::strcpy(new_string.get(), dest->data());
    dest->m_data.swap(new_string);
    dest->m_allocated = new_length;
  }

  std::strcat(dest->data(), append.data());
  dest->m_strlen = new_length;
}

void string_literal::construct(const char_t* str, size_t allocated_length)
{
  assert(str != nullptr);

  if (m_strlen > allocated_length)
    allocated_length = m_strlen;

  m_allocated = allocated_length;
  m_data = shared_ptr<char_t>(allocate_string(allocated_length));
  std::strcpy(m_data.get(), str);
}
string_literal::string_literal(const char_t* str)
  : m_strlen(std::strlen(str))
{
  construct(str, m_strlen);
}
string_literal::string_literal(string_literal&& rhs)
  : m_data(rhs.m_data)
  , m_strlen(rhs.m_strlen)
  , m_allocated(rhs.m_allocated) {}
string_literal::string_literal(initializer_list<string_literal> list)
: m_strlen(0)
{
  if (list.size() == 0)
  {
    return;
  }

  auto get_length = [&list](){
    auto it = list.begin();
    size_t length = 0;
    while (it != list.end()) { length += it->length(); ++it; }
    return length;
  };

  construct("", get_length());
  auto it = list.begin();
  while (it != list.end()) { strcat(this, *it); ++it; }
}

string_literal& string_literal::operator=(string_literal&& rhs)
{
  m_data = rhs.m_data;
  m_strlen = rhs.m_strlen;
  m_allocated = rhs.m_allocated;
  return *this;
}

void string::register_string(string_literal value, string_hash hash)
{
  auto current_value = registry.find(hash);
  if (current_value == registry.end()) //no value exists yet, create a copy
  {
    registry[hash] = value;
  }
#ifdef STRING_HASH_DEBUGGING
  else //then a value for that string exists
  {
    //trivial check
    assert(current_value->second.length() == value.length());

    //compare strings, error if different (2 strings hash to same value)
    assert(std::strcmp(current_value->second.data(), value.data()) == 0);
  }
#endif
}

string_literal& string::access_registry(string_hash hash) {
  auto value = registry.find(hash);
  assert(value != registry.end());
  return value->second;
}

string::string(const string_literal& value)
  : m_hash(hash_function::generate_hash(value.data())) 
{
  register_string(value, m_hash);
}
string::string(const char_t* value)
  : m_hash(hash_function::generate_hash(value))
{
  register_string(value, m_hash);
}

string::string(string&& rhs)
  : m_hash(rhs.m_hash) {}

string& string::operator=(string&& rhs)
{
  m_hash = rhs.m_hash;
  return *this;
}

bool string::operator==(const string& rhs) const
{
  return m_hash == rhs.m_hash;
}

char_t& string::operator[](size_t index) { return access_registry(m_hash).data()[index]; }
const char_t& string::operator[](size_t index) const { return const_cast<string&>(*this)[index]; }

string_hash string::hash() const { return m_hash; }
size_t string::length() const { return access_registry(m_hash).length(); }
const char_t* string::c_str() const { return access_registry(m_hash).data(); }
}
}