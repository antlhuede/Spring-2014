#pragma once

namespace meta
{
template <class U, class... Args>
variant event::trigger(U* obj, Args... args) const
{
  return function.call(obj, std::forward<Args>(args)...);
}
template <class U, class... Args>
variant event::trigger(const U* obj, Args... args) const
{
  assert(function.traits().isConst == true);
  return function.call(obj, std::forward<Args>(args)...);
}
}