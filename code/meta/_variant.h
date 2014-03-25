#pragma once

namespace meta {
class variant_ref {};

class variant
{
public:
  variant();
  variant(const type& type);
  template <class T>
  variant(const T& obj);
  variant(const variant& rhs);
  variant(const variant_ref& rhs);
  variant(const char_t* str);
  variant(const type& type, const void* source, bool take_ownership = false);

  ~variant();

  variant& operator=(const variant& rhs);
  
  template <class T> bool is_type() const;
  template <class T> bool is_same_as(const T& rhs) const;
  template <class T> T get_as() const;

  bool can_convert(const type& t) const;

  template <class T>
  void take_ownership(T* pData);
  void take_ownership(const type& type, void* pData);
  void take_ownership(variant_ref& data);

  const string to_string() const;

private:
  void copy_memory(const type& type, const void* memory);
  void destroy();
  static const unsigned SMALL_OBJECT_SIZE = 16;
  char m_array[SMALL_OBJECT_SIZE];
  void* m_data = nullptr;
  const type* m_type = nullptr;
  bool m_ownMemory;

public:
  const type& type() const { return *m_type; }
  const void* data() const { return m_data; }
  void* data() { return m_data; }

  friend class variant_ref;
};
}