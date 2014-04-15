#pragma once

namespace meta
{
template <class U, class... Args>
variant event::trigger(U* obj, Args... args) const
{
  //bool correct_types = m_function.check_types<U, Args...>();
  //assert(correct_types == true);
  //
  return m_function.call(obj, std::forward<Args>(args)...);
}
template <class U, class... Args>
variant event::trigger(const U* obj, Args... args) const
{
  //bool correct_types = m_function.check_types<U, Args...>();
  //assert(correct_types == true);
  //
  assert(m_function.traits().isConst == true);
  return m_function.call(obj, std::forward<Args>(args)...);
}
}