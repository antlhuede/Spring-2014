#pragma once

#include "meta\_function_internal_inl.h"

namespace meta {

template <class T> 
function::function(T func)
  : m_traits(func), m_initialized(true), m_object(&func)
  , m_caller(&internal::function_traits_deducer<T>::Call)
  , m_checker(&internal::function_traits_deducer<T>::CheckArgs)
{
  typedef decltype(&T::operator()) func_type;
  m_function = new internal::function_holder<func_type>(&T::operator());
}
template <class R, class... Args>
function::function(R(*func)(Args...))
  : m_traits(func), m_initialized(true), m_object(nullptr)
{
  typedef R(*func_type)(Args...);
  typedef internal::function_traits_deducer<func_type> deducer;

  m_caller = &deducer::Call;
  m_checker = &deducer::CheckArgs;
  m_function = new internal::function_holder<func_type>(func);
}
template <class R, class U, class... Args>
function::function(R(U::*func)(Args...), U* obj)
  : m_traits(func), m_initialized(true), m_object(obj)
{
  if (m_object)
    assert(typeof<U>() == m_traits.classType);

  typedef R(U::*func_type)(Args...);
  typedef internal::function_traits_deducer<func_type> deducer;

  m_caller = &deducer::Call;
  m_checker = &deducer::CheckArgs;
  m_function = new internal::function_holder<func_type>(func);
}
template <class R, class U, class... Args>
function::function(R(U::*func)(Args...)const, const U* obj)
  : m_traits(func), m_initialized(true), m_object(const_cast<U*>(obj))
{
  if (m_object)
    assert(typeof<U>() == m_traits.classType);

  typedef R(U::*func_type)(Args...)const;
  typedef internal::function_traits_deducer<func_type> deducer;

  m_caller = &deducer::Call;
  m_checker = &deducer::CheckArgs;
  m_function = new internal::function_holder<func_type>(func);
}

inline function::function(const function& other)
  : m_function(other.m_function->clone())
  , m_traits(other.m_traits)
  , m_caller(other.m_caller)
  , m_checker(other.m_checker)
  , m_initialized(true)
  , m_object(other.m_object) {}

inline function::function(function&& other)
  : m_function(other.m_function)
  , m_traits(other.m_traits)
  , m_caller(other.m_caller)
  , m_checker(other.m_checker)
  , m_initialized(true)
  , m_object(other.m_object)
{
  other.m_object = nullptr;
  other.m_function = nullptr;
  other.m_initialized = false;
}
inline function::~function() { if (m_function) delete m_function; }

template <class... Args>
void function::operator()(Args&&... args) const
{
  assert(sizeof...(Args) == m_traits.numArguments);
  assert(m_traits.hasReturnValue == false);
  const int size = sizeof...(args)+1;
  void* args_ptr[size] = { &args... };
  const type* types_ptr[size] = { typeof(args)... };
  assert(m_checker && m_checker(types_ptr));
  m_caller(m_function, m_object, m_traits.args, args_ptr);
}
template <class U, class... Args>
void function::call(U* object, Args&&... args)
{
  assert(typeof<U>() == m_traits.classType);
  void* temp_obj = m_object;
  m_object = object;
  (*const_cast<const function*>(this))(std::forward<Args&&>(args)...);
  m_object = temp_obj;
}

template <class U, class... Args>
void function::call(const U* object, Args&&... args) const
{
  assert(m_traits.isConst == true);
  assert(typeof<U>() == m_traits.classType);
  call(const_cast<U*>(object), std::forward<Args&&>(args)...);
}
template <class U, class... Args>
bool function::check_types() const
{
  if(sizeof...(Args) != m_traits.numArguments)
    return false;
  if (m_checker == nullptr)
    return false;

  const int size = sizeof...(args)+1;
  const type* types_ptr[size] = { typeof(args)... };

  if (m_checker(types_ptr) == false)
    return false;

  return true;
}


template <class T>
function_traits::function_traits(T func)
  : isMemberFunction(internal::function_traits_deducer<T>::is_member_func)
  , isConst(internal::function_traits_deducer<T>::is_const)
  , hasReturnValue(internal::function_traits_deducer<T>::has_return_value)
  , isLambda(internal::function_traits_deducer<T>::is_lambda)
  , classType(typeof<internal::function_traits_deducer<T>::class_type>())
  , returnType(typeof<internal::function_traits_deducer<T>::return_type>())
  , numArguments(internal::function_traits_deducer<T>::num_args)
{
  internal::function_traits_deducer<T>::DeduceArgs(args);
}
inline bool function_traits::operator==(const function_traits& rhs) const
{
  if (isMemberFunction != rhs.isMemberFunction ||
    isConst != rhs.isConst ||
    hasReturnValue != rhs.hasReturnValue ||
    isLambda != rhs.isLambda ||
    numArguments != rhs.numArguments)
    return false;

  if ((classType != rhs.classType) ||
      (returnType != rhs.returnType))
    return false;

  for (unsigned i = 0; i < numArguments; ++i)
    if (args[i].type != rhs.args[i].type)
      return false;

  return true;
}
inline bool function_traits::operator!=(const function_traits& rhs) const
{
  return !((*this) == rhs);
}
}