#pragma once
//#include "../core/types.h"
//#include "hash_map.h"

//this define causes the string pool to assert on string hash collisions
#define STRING_HASH_DEBUGGING

namespace alib
{
  namespace containers
  {
    typedef hash_map<string_hash, char*> string_pool;

    //template <typename
    //template <typename... Args>
    //string strcat(Args... args)
    //{
    //  
    //}

    class string
    {
    public:
      string();
      string(const char* value);
      string(const string& rhs);
      string(string&& rhs);

      string& operator=(const string& rhs);
      string& operator=(string&& rhs);

      bool operator==(const string& rhs) const;

      char& operator[](size index);
      const char& operator[](size index) const;
  
      string_hash hash() const;
      size length() const;
      const char* c_str() const;
    private:
      inline string& copy_string(const string& rhs)
      {
        m_length = rhs.m_length;
        m_hash = rhs.m_hash;
        return *this;
      }
      static string_pool registry;
      static void register_string(const char* value, size length, string_hash hash);
      string_hash m_hash;
      size m_length;
    };
  }
}