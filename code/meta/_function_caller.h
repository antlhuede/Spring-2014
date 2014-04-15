#pragma once

namespace meta { namespace internal {
//caller: used to correctly cast the base function pointer and call the function pointer
//lambda functions fall under the category of const member function

template <class R, class T> struct caller;

template <class R, class... Args>
struct caller<R, R(*)(Args...)>
{
  typedef function_holder<R(*)(Args...)> func_holder;

  static inline R Call(base_function* function, void* object, Args... parameters)
  {
    return static_cast<func_holder*>(function)->function_ptr(std::forward<Args>(parameters)...);
  }
};
template <class... Args>
struct caller<void_, void(*)(Args...)>
{
  typedef function_holder<void(*)(Args...)> func_holder;

  static inline void_ Call(base_function* function, void* object, Args... parameters)
  {
    static_cast<func_holder*>(function)->function_ptr(std::forward<Args>(parameters)...);
    return void_();
  }
};

template <class R, class U, class... Args>
struct caller<R, R(U::*)(Args...)>
{
  typedef function_holder<R(U::*)(Args...)> func_holder;

  static inline R Call(base_function* function, void* object, Args... parameters)
  {
    return (static_cast<U*>(object)->*(static_cast<func_holder*>(function)->function_ptr))(std::forward<Args>(parameters)...);
  }
};
template <class U, class... Args>
struct caller<void_, void(U::*)(Args...)>
{
  typedef function_holder<void(U::*)(Args...)> func_holder;

  static inline void_ Call(base_function* function, void* object, Args... parameters)
  {
    (static_cast<U*>(object)->*(static_cast<func_holder*>(function)->function_ptr))(std::forward<Args>(parameters)...);
    return void_();
  }
};

template <class R, class U, class... Args>
struct caller<R, R(U::*)(Args...)const>
{
  typedef function_holder<R(U::*)(Args...)const> func_holder;

  static inline R Call(base_function* function, void* object, Args... parameters)
  {
    return (static_cast<const U*>(object)->*(static_cast<func_holder*>(function)->function_ptr))(std::forward<Args>(parameters)...);
  }
};
template <class U, class... Args>
struct caller<void_, void(U::*)(Args...)const>
{
  typedef function_holder<void(U::*)(Args...)const> func_holder;

  static inline void_ Call(base_function* function, void* object, Args... parameters)
  {
    (static_cast<const U*>(object)->*(static_cast<func_holder*>(function)->function_ptr))(std::forward<Args>(parameters)...);
    return void_();
  }
};
}}