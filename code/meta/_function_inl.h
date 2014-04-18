#pragma once

#include "meta\_function_internal_inl.h"

namespace meta {
namespace internal
{
  static bool DefaultArgsChecker(const type**, size_t) { return false; }
  static variant DefaultCaller(base_function*, void*, const arg_traits*, void**) { return variant(); }
}

inline function::function()
  : traits(), initialized(false), CheckArgs(&internal::DefaultArgsChecker)
  , m_object(nullptr), m_function(nullptr), m_caller(&internal::DefaultCaller) {}

template <class T> 
function::function(T lambda)
  : traits(lambda), initialized(true)
{
  Construct(lambda, &lambda);
}
template <class R, class... Args>
function::function(R(*func)(Args...))
  : traits(func), initialized(true)
{
  Construct(func);
}
template <class R, class U, class... Args>
function::function(R(U::*func)(Args...), U* object)
  : traits(func), initialized(true)
{
  Construct(func, object);
}

template <class R, class U, class... Args>
function::function(R(U::*func)(Args...)const, const U* object)
  : traits(func), initialized(true)
{
  Construct(func, const_cast<U*>(object));
}

template <class T>
void function::Construct(T func, void* object)
{
  typedef internal::function_descriptor<T> descriptor;
  
  const_cast<ArgChecker&>(CheckArgs) = &descriptor::CheckArgs;

  m_function = descriptor::Create(func);
  m_caller = &descriptor::Call;
  m_object = object;
}

inline function::function(const function& other)
  : m_function(other.m_function ? other.m_function->clone() : nullptr)
  , traits(other.traits)
  , CheckArgs(other.CheckArgs)
  , initialized(other.initialized)
  , m_caller(other.m_caller)
  , m_object(other.m_object) {}

inline function::function(function&& other)
  : m_function(other.m_function)
  , traits(other.traits)
  , CheckArgs(other.CheckArgs)
  , initialized(other.initialized)
  , m_caller(other.m_caller)
  , m_object(other.m_object)
{
  other.m_object = nullptr;
  other.m_function = nullptr;
  const_cast<bool&>(other.initialized) = false;
}
inline function::~function() { if (m_function) delete m_function; }

#define IMPLEMENT_CALLER_BODY()                               \
  const int numArgs = sizeof...(Args);                        \
  assert(numArgs == traits.numArguments);                     \
  const int size = numArgs+1;                                 \
  /*make const now, so we can store pointers to const args*/  \
  const void* args_ptr[size] = { &args... };                  \
  const type* types_ptr[size] = { typeof(args)... };          \
  assert(CheckArgs && CheckArgs(types_ptr, numArgs))

template <class... Args>
variant function::operator()(Args... args) const
{
  IMPLEMENT_CALLER_BODY();
  //cast away the const from the args since its expected by the caller
  return m_caller(m_function, m_object, traits.args, const_cast<void**>(args_ptr));
}
template <class U, class... Args>
variant function::Call(U* object, Args... args) const
{
  IMPLEMENT_CALLER_BODY();
  //cast away the const from the args since its expected by the caller
  return m_caller(m_function, object, traits.args, const_cast<void**>(args_ptr));
}
#undef IMPLEMENT_CALLER_BODY

template <class U, class... Args>
variant function::Call(const U* object, Args... args) const
{
  assert(traits.isConst == true);
  return const_cast<function*>(this)->Call(const_cast<U*>(object), std::forward<Args>(args)...);
}
inline variant function::CallGeneric(const type* classType, void* object, size_t numArgs, const type** argTypes, const void** args) const
{
  assert(traits.classType == classType);
  assert(CheckArgs && CheckArgs(argTypes, numArgs));
  return m_caller(m_function, object, traits.args, const_cast<void**>(args));
}
inline variant function::CallGeneric(const type* classType, const void* object, size_t numArgs, const type** argTypes, const void** args) const
{
  assert(traits.isConst == true);
  return CallGeneric(classType, const_cast<void*>(object), numArgs, argTypes, args);
}
//template <class U, class... Args>
//bool function::check_types() const
//{
//  if(sizeof...(Args) != m_traits.numArguments)
//    return false;
//  if (m_checker == nullptr)
//    return false;
//
//  const int size = sizeof...(args)+1;
//  const type* types_ptr[size] = { typeof(args)... };
//
//  if (m_checker(types_ptr) == false)
//    return false;
//
//  return true;
//}

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