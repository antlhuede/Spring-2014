#pragma once

namespace alib
{
  namespace core
  {
    namespace hash_functions
    {
      class x65599
      {
      public:
        static string_hash generate_hash(const char* str);

      private:
        static string_hash do_hash(const char* str, string_hash hash);
      };

      class FNV
      {
      public:
        static string_hash generate_hash(const char* str);
      };

      class sum
      {
      public:
        static string_hash generate_hash(const char* str);
      };
    }
  }

}
