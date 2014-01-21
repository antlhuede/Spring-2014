#pragma once
//#include "../core/types.h"
//#include "hash_map.h"

//this define causes the string pool to assert on string hash collisions
#define STRING_HASH_DEBUGGING

namespace alib
{
  namespace containers
  {
    class string_literal
    {
    public:
        string_literal(const char* = "");
        ~string_literal() = default;
        string_literal(initializer_list<string_literal> list);

        string_literal(const string_literal&) = default;
        string_literal& operator=(const string_literal&) = default;

        string_literal(string_literal&&);
        string_literal& operator=(string_literal&&);

        inline char* get() { return m_data.get(); }
        inline const char* get() const { return m_data.get(); }
        inline size_t length() const { return m_strlen; }
    private:
        void construct(const char*, size_t allocated_length);
        shared_ptr<char> m_data;
        size_t m_strlen;
        size_t m_allocated;
        friend void strcat(string_literal* dest, const string_literal& append);
    };

    void strcat(string_literal* dest, const string_literal& append);
    typedef hash_map<string_hash, string_literal> string_pool;

    class string
    {
    public:
      string(const string_literal& value = "");
      string(const char* value); //char* conversion constructor... nice to have
      ~string() = default;

      string(const string& rhs) = default;
      string& operator=(const string& rhs) = default;

      string(string&& rhs);
      string& operator=(string&& rhs);

      bool operator==(const string& rhs) const;

      char& operator[](size_t index);
      const char& operator[](size_t index) const;
  
      string_hash hash() const;
      size_t length() const;
      const char* c_str() const;

    private:
      static string_pool registry;
      static void register_string(string_literal value, string_hash hash);
      static string_literal& access_registry(string_hash hash);

      string_hash m_hash;
    };
  }
}