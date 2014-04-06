#pragma once

namespace meta {
class variant_ref;

class variant
{
public:
  variant();
  template <class T>
  variant(const T& obj);
  variant(const variant& other);
  variant(variant&& other);
  variant(const variant_ref& other);
  variant(const char_t* str);
  explicit variant(const type& type, const void* obj = nullptr);

  ~variant();

  variant& operator=(const variant& rhs);
  
  template <class T> bool is_type() const;
  template <class T> bool is_same_as(const T& rhs) const;
  template <class T> const T& get_as() const;
  template <class T> T& get_as(); 

  bool can_convert(const type& to_type) const;

  template <class T>
  void take_ownership(T* pData);
  //void take_ownership(const type& type, void* pData);
  void take_ownership(variant_ref& data);

  const string to_string() const;

private:
  void construct(const type& type, const void* memory);
  void destroy();
  static const unsigned SMALL_OBJECT_SIZE = 16;
  char m_array[SMALL_OBJECT_SIZE];
  void* m_data = nullptr;
  const type* m_type = nullptr;

public:
  const type& type() const { return *m_type; }
  const void* data() const { return m_data; }
  void* data() { return m_data; }

  friend class variant_ref;
};

class variant_ref
{
public:
  template <class T> variant_ref(const T& obj);

  variant_ref();
  variant_ref(const variant& other);
  variant_ref(const variant_ref& other);
  variant_ref(variant_ref&& other);
  ~variant_ref() = default;

  variant_ref& operator=(const variant& other);
  variant_ref& operator=(const variant_ref& other);

  template <class T> bool is_type() const;
  template <class T> bool is_same_as(const T& other) const;
  template <class T> const T& get_as() const;
  template <class T> T& get_as();

  template <class T> bool can_convert() const;
  bool can_convert(const type& to_type) const;

private:
  void* m_data = nullptr;
  const type* m_type = nullptr;

public:
  const void* data() const { return m_data; }
  void* data() { return m_data; }
  const meta::type& type() const { return *m_type; }
};
}