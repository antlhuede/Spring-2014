#pragma once

namespace meta {
class field
{
public:
  field() = default;
  ~field() = default;
  field(const string& name_, const ::meta::type* type_, size_t offset_)
    : name(name_), type(type_), offset(offset_) {}

  void* field::member_ptr(void* object_ptr) const
  {
    return ((char*)object_ptr) + offset;
  }
  const void* field::member_ptr(const void* object_ptr) const
  {
    return member_ptr(const_cast<void*>(object_ptr));
  }
  const string field::to_string(void* var) const
  {
    return type->toString(member_ptr(var));
  }

  const string name = "uninitialized_field";
  const ::meta::type* const type = typeof<nulltype>();
  const size_t offset = 0;
};
}