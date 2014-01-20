#pragma once

namespace alib
{
  namespace containers
  {
    template <class Key, class Value>
    class hash_map
    {
    public:
      hash_map();
      hash_map(const hash_map& rhs);

      hash_map(hash_map&&);
      hash_map& operator=(hash_map&&);

    private:
      
    };
  }
}