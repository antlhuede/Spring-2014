#pragma once

namespace meta {

class field
{
public:
  field(const string& name, const ::meta::type& type, size_t offset)
    : m_name(name), m_type(type), m_offset(offset) {}

  const string& name() const { return m_name; }
  const type& type() const { return m_type; }
  size_t offset() const { return m_offset; }

  void* field::member_ptr(void* object_ptr) const
  {
    return ((char*)object_ptr) + m_offset;
  }
  const string field::to_string(void* var) const
  {
    return m_type.to_string(member_ptr(var));
  }

private:
  string m_name = "uninitialized_field";
  ::meta::type m_type;
  size_t m_offset = 0;
};

}