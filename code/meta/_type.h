#pragma once

namespace meta
{
  class type;
  class field;
  class nulltype {};
  class variant {};
  template <class T>
  struct decl {};
  
  typedef void(*ReadFunc)(istream&, void*);
  typedef void(*WriteFunc)(ostream&, void*);
  typedef void(*PlacementNewFunc)(void*);
  typedef void(*CopyFunc)(void* dest, void* source);
  typedef void(*MoveFunc)(void* dest, void* source);
  typedef void(*DestructFunc)(void*);
  typedef const string(*StringizeFunc)(void*);

  class type
  {
  public:

    template <class T>
    type(decl<T>, const string name, ReadFunc r, WriteFunc w, StringizeFunc str);

    type& operator=(const type& rhs);

    bool operator==(const type& rhs) const;

    const ReadFunc read;
    const WriteFunc write;
    const StringizeFunc to_string;

    const PlacementNewFunc construct;
    const CopyFunc copy;
    const MoveFunc move;
    const DestructFunc destruct;

    template <typename T, typename U >
    type& Field(const string& name, T U::*var);

    const field field(const string& name, bool assert_on_failure = false) const;

    const vector<const ::meta::field*>& fields() const { return m_fields; }
    size_t size() const { return m_size; }
    const string& name() const { return m_name; }
    unsigned id() const { return m_id; }

  private:
    static unsigned S_ID_COUNTER;
    unsigned m_id = 0;
    size_t m_size = 0;
    string m_name = "uninitialized_type";
    vector<const ::meta::field*> m_fields;
    hash_map<string, size_t> m_fieldmap;
  };

  class field
  {
  public:
    field(const string& name, const type& type, size_t offset);

    const string& name() const;
    const type& type() const;
    size_t offset() const;

    void* member_ptr(void* object_ptr) const;
    const string to_string(void* var) const;

  private:
    string m_name;
    ::meta::type m_type;
    size_t m_offset;
  };
  
  template <class T>
  struct meta_holder { static const type t; };

  struct meta_registry 
  {
  public:
    static const type& find(const string& key);
    
    static bool register_type(const type* t);
  private:
    static hash_map<string, const type*> m_registry; 
  };
    
  template <class T>
  const string to_string_basic_types(void* var);

  template <class T>
  void read_basic_types(istream& stream, void* memory);
  template <class T>
  void write_basic_types(ostream& stream, void* memory);

  
  template <class T>
  const string to_string_objects(void* memory);

  template <class T>
  void read_objects(istream& stream, void* memory);
  template <class T>
  void write_objects(ostream& stream, void* memory);

  struct converter
  {
    static int toInt(const type& t, void* obj);
    static bool toBool(const type& t, void* obj);
    static float toFloat(const type& t, void* obj);
    static double toDouble(const type& t, void* obj);
    static string toString(const type& t, void* obj);

    template <typename T>
    static T convert(const type& t, void* obj);

    template <typename T>
    static bool safe_convert(const type& t, void* obj, T* result);
  };
}

#include "meta\_type_inl.h"
