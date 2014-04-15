#pragma once

namespace meta {

class type : public list<type>
{
public:
  template <class T>
  explicit type(decl<T>, const string name, ReadFunc r, WriteFunc w, StringizeFunc str);
  ~type() = default;

  type(type&&) = delete;
  type(const type& rhs) = delete;
  type& operator=(const type& rhs) = delete;

  void read(istream& stream, void* memory) const;
  void write(ostream& stream, void* memory) const;
  void construct(void* memory) const;
  void destruct(void* memory) const;
  void copy(void* dest, const void* source) const;
  const string to_string(const void* memory) const;
  void* clone(const void* source) const;
  void* allocate() const;
  void deallocate(void* memory) const;
  void assign(void* dest, const void* source) const;

  bool operator==(const type& rhs) const;
  bool operator!=(const type& rhs) const;

  template <class T, class U>
  type& Field(const string& name, T U::*var);

  template <class T, class U>
  type& Property(const string& name, T(U::*get)()const, void(U::*set)(T));
  
  template <class T, class U>
  type& Property(const string& name, void*, void(U::*set)(T));
  template <class T, class U>
  type& Property(const string& name, T(U::*get)()const, void*);
  //template <class U, class R, class... Args>
  //type& Event(const string& name, R(U::*func)(Args...));

  const vector<const field>& fields() const;
  size_t size() const { return m_size; }
  const string& name() const { return m_name; }
  unsigned id() const { return m_id; }

  const field field(const string& name, bool assert_on_failure = false) const;
  const property property(const string& name, bool assert_on_failure = false) const;

private:
  static unsigned S_ID_COUNTER;
  unsigned m_id = 0;
  size_t m_size = 0;
  string m_name = "nulltype";

  vector<const ::meta::field> m_fields;
  hash_map<string, size_t> m_fieldMap;

  vector<const ::meta::property> m_properties;
  hash_map<string, size_t> m_propertyMap;

  vector<const ::meta::event> m_events;
  hash_map<string, size_t> m_eventMap;

  const ReadFunc m_read = nullptr;
  const WriteFunc m_write = nullptr;
  const StringizeFunc m_to_string = nullptr;

  const PlacementNewFunc m_construct = nullptr;
  const CopyFunc m_copy = nullptr;
  const DestructFunc m_destruct = nullptr;
  const AllocFunc m_allocate = nullptr;
  const DeleteFunc m_deallocate = nullptr;
  const CloneFunc m_clone = nullptr;
  const AssignFunc m_assign = nullptr;
};

}
