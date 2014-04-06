#pragma once

namespace meta {

class type : public list<type>
{
public:
  template <class T>
  type(decl<T>, const string name, ReadFunc r, WriteFunc w, StringizeFunc str);
  type(const type& rhs) = default;
  ~type() = default;

  type& operator=(const type& rhs);

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

  template <typename T, typename U >
  type& Field(const string& name, T U::*var);

  const vector<const field>& fields() const;
  size_t size() const { return m_size; }
  const string& name() const { return m_name; }
  unsigned id() const { return m_id; }

  const field field(const string& name, bool assert_on_failure = false) const;
private:
  static unsigned S_ID_COUNTER;
  unsigned m_id = 0;
  size_t m_size = 0;
  string m_name = "nulltype";
  vector<const ::meta::field> m_fields;
  hash_map<string, size_t> m_fieldmap;

  const ReadFunc m_read;
  const WriteFunc m_write;
  const StringizeFunc m_to_string;

  const PlacementNewFunc m_construct;
  const CopyFunc m_copy;
  const DestructFunc m_destruct;
  const AllocFunc m_allocate;
  const DeleteFunc m_deallocate;
  const CloneFunc m_clone;
  const AssignFunc m_assign;
};

}
