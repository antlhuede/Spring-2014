#pragma once

#include "meta\_function_internal_inl.h"

namespace meta {

template <class T> 
function::function(T lambda)
  : m_traits(lambda), m_initialized(true), m_object(&lambda)
{
  construct(lambda);
}
template <class R, class... Args>
function::function(R(*func)(Args...))
  : m_traits(func), m_initialized(true), m_object(nullptr)
{
  construct(func);
}
template <class R, class U, class... Args>
function::function(R(U::*func)(Args...), U* obj)
  : m_traits(func), m_initialized(true), m_object(obj)
{
  construct(func);
}
template <class R, class U, class... Args>
function::function(R(U::*func)(Args...)const, const U* obj)
  : m_traits(func), m_initialized(true), m_object(const_cast<U*>(obj))
{
  construct(func);
}

template <class T>
void function::construct(T func)
{
  typedef internal::function_descriptor<T> descriptor;

  m_function = descriptor::Create(func);
  m_caller = &descriptor::Call;
  m_checker = &descriptor::CheckArgs;
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
variant function::operator()(Args... args) const
{
  assert(sizeof...(Args) == m_traits.numArguments);
  const int size = sizeof...(args)+1;
  //make const now, so we can store pointers to const args
  const void* args_ptr[size] = { &args... };
  const type* types_ptr[size] = { typeof(args)... };
  assert(m_checker && m_checker(types_ptr));
  //cast away the const from the args since its expected by the caller
  return m_caller(m_function, m_object, m_traits.args, const_cast<void**>(args_ptr));
}
template <class U, class... Args>
variant function::call(U* object, Args... args) const
{
  assert(sizeof...(Args) == m_traits.numArguments);
  assert(typeof<U>() == m_traits.classType);
  const int size = sizeof...(args)+1;
  //make const now, so we can store pointers to const args
  const void* args_ptr[size] = { &args... };
  const type* types_ptr[size] = { typeof(args)... };
  assert(m_checker && m_checker(types_ptr));
  //cast away the const from the args since its expected by the caller
  return m_caller(m_function, object, m_traits.args, const_cast<void**>(args_ptr));
}

template <class U, class... Args>
variant function::call(const U* object, Args... args) const
{
  assert(m_traits.isConst == true);
  return const_cast<function*>(this)->call(const_cast<U*>(object), std::forward<Args&&>(args)...);
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
  : isMemberFunction(internal::function_descriptor<T>::is_member_function)
  , isConst(internal::function_descriptor<T>::is_const)
  , hasReturnValue(internal::function_descriptor<T>::has_return_value)
  , isLambda(internal::function_descriptor<T>::is_lambda)
  , classType(typeof<internal::function_descriptor<T>::class_type>())
  , returnType(typeof<internal::function_descriptor<T>::return_type>())
  , numArguments(internal::function_descriptor<T>::num_args)
{
  internal::function_descriptor<T>::DeduceArgs(args);
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