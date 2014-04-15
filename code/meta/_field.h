#pragma once

namespace meta {
class field
{
public:
  field() = default;
  field(const string& name, const ::meta::type* type, size_t offset)
    : m_name(name), m_type(type), m_offset(offset) {}

  const string& name() const { return m_name; }
  const type* type() const { return m_type; }
  size_t offset() const { return m_offset; }

  void* field::member_ptr(void* object_ptr) const
  {
    return ((char*)object_ptr) + m_offset;
  }
  const void* field::member_ptr(const void* object_ptr) const
  {
    return member_ptr(const_cast<void*>(object_ptr));
  }
  const string field::to_string(void* var) const
  {
    return m_type->to_string(member_ptr(var));
  }

private:
  string m_name = "uninitialized_field";
  const ::meta::type* m_type = typeof<nulltype>();
  size_t m_offset = 0;
};
}