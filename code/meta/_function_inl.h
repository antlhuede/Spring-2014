#pragma once

#include "meta\_function_internal_inl.h"
namespace meta {
struct base_function { virtual ~base_function() {} };
template <class T>
struct function_holder : public base_function
{
  function_holder(T function_) : function_ptr(function_) {}
  T function_ptr;
};

//template <class T>
//function::function(T& func)
//: m_traits(func), m_initialized(true), m_object(&func)
//, m_function(new function_holder<decltype(&T::operator())>(func))
//, m_caller(&internal::function_traits_deducer<T>::call)
//, m_checker(&internal::function_traits_deducer<T>::check_args) {}

template <class T> struct base_creator
{
  static base_function* create(T func, void**)
  {
    return new function_holder<T>(func);
  }
};
template <class T> struct func_creator;

//creator for functors / lambda objects
template <class T> struct func_creator
{
  static base_function* create(T& func, void** pObj)
  {
    *pObj = &func;
    return new function_holder<decltype(&T::operator())>(&T::operator());
  }
};
template <class R, class... Args> struct func_creator<R(*)(Args...)> : public base_creator<R(*)(Args...)>{};
template <class U, class R, class... Args> struct func_creator<R(U::*)(Args...)> : public base_creator<R(U::*)(Args...)>{};
template <class U, class R, class... Args> struct func_creator<R(U::*)(Args...)const> : public base_creator<R(U::*)(Args...)const>{};

template <class T> 
function::function(T func, void* obj)
  : m_traits(func), m_initialized(true), m_object(obj)
  , m_caller(&internal::function_traits_deducer<T>::call)
  , m_checker(&internal::function_traits_deducer<T>::check_args)
{
  m_function = func_creator<T>::create(func, &m_object);
}

inline function::~function() { if (m_function) delete m_function; }

template <class... Args>
void function::operator()(Args... args)
{
  assert(sizeof...(Args) == m_traits.numArguments);
  assert(m_traits.hasReturnValue == false);
  const int size = sizeof...(args)+1;
  void* args_ptr[size] = { &args... };
  const type* types_ptr[size] = { &typeof(args)... };
  assert(m_checker && m_checker(types_ptr));
  m_caller(m_function, m_object, m_traits.args, args_ptr);
}

template <class T>
function_traits::function_traits(T func)
  : isMemberFunction(internal::function_traits_deducer<T>::is_member_func)
  , isConst(internal::function_traits_deducer<T>::is_const)
  , hasReturnValue(internal::function_traits_deducer<T>::has_return_value)
  , classType(&typeof<internal::function_traits_deducer<T>::class_type>())
  , returnType(&typeof<internal::function_traits_deducer<T>::return_type>())
  , numArguments(internal::function_traits_deducer<T>::num_args)
{
  internal::function_traits_deducer<T>::deduce_args(args);
}
inline bool function_traits::operator==(const function_traits& rhs) const
{
  if (isMemberFunction != rhs.isMemberFunction ||
    isConst != rhs.isConst ||
    hasReturnValue != rhs.hasReturnValue ||
    numArguments != rhs.numArguments)
    return false;

  if ((isMemberFunction && *classType != *rhs.classType) ||
    (hasReturnValue && *returnType != *rhs.returnType))
    return false;

  for (unsigned i = 0; i < numArguments; ++i)
    if (*args[i].type != *rhs.args[i].type)
      return false;

  return true;
}
}