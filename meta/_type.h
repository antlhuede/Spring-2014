#pragma once

namespace alib
{
  namespace meta
  {
    
    class type
    {
    public:
      type() = default;

      size_t size() const { return m_size; }
      const string& name() const { return m_name; }
    private:
      size_t m_size = 0;
      string m_name = "uninitialized_type";
      //properties
      //fields
      //events
      //etc
    };

    class type_info {};
  }
}