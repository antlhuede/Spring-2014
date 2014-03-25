#pragma once

namespace meta {

class type : public list<type>
{
public:
  template <class T>
  type(decl<T>, const string name, ReadFunc r, WriteFunc w, StringizeFunc str);
  type(const type& rhs) = default;

  type& operator=(const type& rhs);

  bool operator==(const type& rhs) const;
  bool operator!=(const type& rhs) const;

  const ReadFunc read;
  const WriteFunc write;
  const StringizeFunc to_string;

  const PlacementNewFunc construct;
  const CopyFunc copy;
  const DestructFunc destruct;
  const AllocFunc make_new;
  const DeleteFunc delete_ptr;
  const CloneFunc clone;

  template <typename T, typename U >
  type& Field(const string& name, T U::*var);

  const vector<shared_ptr<const field>>& fields() const { return m_fields; }
  size_t size() const { return m_size; }
  const string& name() const { return m_name; }
  unsigned id() const { return m_id; }

  const field field(const string& name, bool assert_on_failure = false) const;
private:
  static unsigned S_ID_COUNTER;
  unsigned m_id = 0;
  size_t m_size = 0;
  string m_name = "nulltype";
  vector<shared_ptr<const ::meta::field>> m_fields;
  hash_map<string, size_t> m_fieldmap;
};

}
