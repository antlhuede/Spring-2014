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
        typedef uint32 hash_type;
        static hash_type generate_hash(const char_t* str);

      private:
        static hash_type do_hash(const char_t* str, hash_type hash);
      };

      class FNV32
      {
      public:
        typedef uint32 hash_type;
        static hash_type generate_hash(const char_t* str);
      };

      class FNV64
      {
      public:
        typedef uint64 hash_type;
        static hash_type generate_hash(const char_t* str);
      };

      class sum
      {
      public:
        typedef uint32 hash_type;
        static hash_type generate_hash(const char_t* str);
      };
    }
  }

}
