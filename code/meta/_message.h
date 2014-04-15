#pragma once

namespace meta {
typedef hash_map<string, vector<function>> message_table;

class messenger
{
public:
  template <class... Args>
  bool DispatchMessage(const string& name, Args&&... args)
  {
    auto item = m_table.find(name);
    if (item == m_table.end())
      return false;

    vector<function>& listeners = item->second;
    for (function& func : listeners)
      func(std::forward<Args&&>(args)...);

    return true;
  }
  
  bool AddListener(const string& name, const function& func)
  {
    auto item = m_table.find(name);
    
    if (item == m_table.end())
    {
      m_table[name] = vector<function>();
      item = m_table.find(name);
    }
    else if (item->second.size())
    {
      const function_traits& root_traits = item->second.front().traits();
      const function_traits& new_traits = func.traits();

      if (root_traits.numArguments != new_traits.numArguments)
        return false;

      for (size_t i = 0; i < root_traits.numArguments; ++i)
      {
        if (*root_traits.args[i].type != *new_traits.args[i].type ||
             root_traits.args[i].isConst != new_traits.args[i].isConst ||
             root_traits.args[i].isPointer != new_traits.args[i].isPointer ||
             root_traits.args[i].isReference != new_traits.args[i].isReference)
          return false;
      }
    }

    item->second.push_back(func);
    return true;
  }

private:
  message_table m_table;
};
}