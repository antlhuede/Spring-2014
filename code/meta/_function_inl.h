#pragma once

namespace meta
{
namespace internal
{

template <class T> struct caller;

template <class R, class... Args>
struct caller<R(*)(Args...)>
{
  typedef function_holder<R(*)(Args...)> FuncType;
  template <class... Args>
  static R call(base_function* function, Args... parameters)
  {
    static_cast<FuncType*>(function)->function_ptr(std::forward<Args>(parameters)...);
  }
};

template <class R, class U, class... Args>
struct caller<R(U::*)(Args...)>
{
  typedef function_holder<R(U::*)(Args...)> FuncType;
  template <class... Args>
  static R call(base_function* function, Args... parameters)
  {
    //static_cast<FuncType*>(function)->function_ptr(std::forward<Args>(parameters)...);
  }
};

template <class R, class U, class... Args>
struct caller<R(U::*)(Args...)const>
{
  typedef function_holder<R(U::*)(Args...)const> FuncType;
  template <class... Args>
  static R call(base_function* function, Args... parameters)
  {
    //static_cast<FuncType*>(function)->function_ptr(std::forward<Args>(parameters)...);
  }
};
#undef IMPLEMENT_CALLER

template <class R, class U, bool is_const, class... Args>
struct function_operator
{
  typedef std::tuple<Args...> arg_tuple;
  static const bool is_member_func = (std::is_same<U, nulltype>::value == false);
  typedef R(*GlobalFuncType)(Args...);
  typedef R(U::*MemberFuncType)(Args...);
  typedef R(U::*ConstMemberFuncType)(Args...)const;

  typedef typename std::conditional<is_member_func == false, GlobalFuncType, 
              typename std::conditional<is_const == false, MemberFuncType, ConstMemberFuncType>::type>::type FuncType;
  
  template <size_t> struct unwrap;
  template <size_t N> struct unwrap
  {
    static void args(arg_traits* args)
    {
      typedef std::tuple_element<sizeof...(Args)-N, arg_tuple>::type ElementType;
      arg_traits& arg = args[sizeof...(Args)-N];
      arg.type = &typeof<ElementType>();
      arg.isPointer = std::is_pointer<ElementType>::value;
      arg.isReference = std::is_reference<ElementType>::value;
      arg.isConst = std::is_const<ElementType>::value;
      unwrap<N - 1>::args(args);
    }
    static bool check_args(const type** arg_types)
    {
      typedef typename std::tuple_element<sizeof...(Args)-N, arg_tuple>::type ExpectedType;
      const type& arg_type = *arg_types[sizeof...(Args)-N];
      const type& expected_type = typeof<ExpectedType>();
      if (arg_type != expected_type)
        return false;
      return unwrap<N - 1>::check_args(arg_types);
    }
    template <class... ArgsT>
    static R call(base_function* function, const arg_traits* traits, void** args, ArgsT... parameters)
    {
      const int index = sizeof...(Args)-N;
      typedef typename std::tuple_element<index, arg_tuple>::type ParameterType;
      if (traits[index].isReference)
      {
        unwrap<N - 1>::call(function, traits, args, std::forward<ArgsT>(parameters)...,
          static_cast<ParameterType>(*static_cast<std::remove_reference<ParameterType>::type*>(args[index])));
      }
      else
      {
        unwrap<N - 1>::call(function, traits, args, std::forward<ArgsT>(parameters)...,
          *static_cast<std::remove_reference<ParameterType>::type*>(args[index]));
      }
    }
  };
  template <> struct unwrap<0>
  {
    static void args(arg_traits* args) { /*end recursion*/ }
    static bool check_args(const type** arg_types) { return true; /*end recursion*/ }
    template <class... ArgsT>
    static R call(base_function* function, const arg_traits* traits, void** args, ArgsT... parameters)
    {
      caller<FuncType>::call(function, std::forward<Args>(parameters)...);
    }
  };
  static void args(arg_traits* args)
  {
    unwrap<sizeof...(Args)>::args(args);
  }
  static bool check_args(const type** arg_types)
  {
    return unwrap<sizeof...(Args)>::check_args(arg_types);
  }
  static void call(base_function* function, const arg_traits* traits, void** args)
  {
    unwrap<sizeof...(Args)>::call(function, traits, args);
  }
};

#define IMPLEMENT_FUNC_TRAITS_DEDUCER(CLASS_TYPE, IS_MEM_FUNC, IS_CONST)                        \
  typedef CLASS_TYPE class_type;                                                                \
  typedef typename std::conditional<std::is_same<R, void>::value, void_, R>::type return_type;  \
  enum {                                                                                        \
    num_args = sizeof...(Args),                                                                 \
    has_return_value = (std::is_same<R, void>::value == false),                                 \
    is_member_func = IS_MEM_FUNC,                                                               \
    is_const = IS_CONST,                                                                        \
  };

template <class R, class... Args>
struct function_traits_deducer<R(*)(Args...)> : public function_operator<R, nulltype, false, Args...>
{
  IMPLEMENT_FUNC_TRAITS_DEDUCER(nulltype, true, false)
};
template <class R, class U, class... Args>
struct function_traits_deducer<R(U::*)(Args...)> : public function_operator<R, U, false, Args...>
{
  IMPLEMENT_FUNC_TRAITS_DEDUCER(U, false, false)
};
template <class R, class U, class... Args>
struct function_traits_deducer<R(U::*)(Args...)const> : public function_operator<R, U, true, Args...>
{
  IMPLEMENT_FUNC_TRAITS_DEDUCER(U, true, true)
};
#undef IMPLEMENT_FUNC_TRAITS_DEDUCER
}

#define IMPLEMENT_FUNCTION_TRAITS_CONSTRUCTOR(TYPE)                             \
  : isMemberFunction(internal::function_traits_deducer<TYPE>::is_member_func)   \
  , isConst(internal::function_traits_deducer<TYPE>::is_const)                  \
  , hasReturnValue(internal::function_traits_deducer<TYPE>::has_return_value)   \
  , classType(&typeof<internal::function_traits_deducer<TYPE>::class_type>())   \
  , returnType(&typeof<internal::function_traits_deducer<TYPE>::return_type>()) \
  , numArguments(internal::function_traits_deducer<TYPE>::num_args)             \
{                                                                               \
  internal::function_traits_deducer<TYPE>::args(args);                          \
}

template <class T>
function_traits::function_traits(T func) IMPLEMENT_FUNCTION_TRAITS_CONSTRUCTOR(T)
template <class T>
function_traits::function_traits(decl<T>) IMPLEMENT_FUNCTION_TRAITS_CONSTRUCTOR(T)
#undef IMPLEMENT_FUNCTION_TRAITS_CONSTRUCTOR

template <class... Args>
void function::operator()(Args... args)
{
  assert(sizeof...(Args) == m_traits.numArguments);
  assert(m_traits.hasReturnValue == false);
  const int size = sizeof...(args)+1;
  void* args_ptr[size] = { &args... };
  const type* types_ptr[size] = { &typeof(args)... };
  assert(m_checker && m_checker(types_ptr));
  m_caller(m_function, m_traits.args, args_ptr);
}
template <class T> function::function(T func, void* obj)
: m_traits(func), m_initialized(true), m_object(obj)
, m_function(new function_holder<T>(func))
, m_caller(&internal::function_traits_deducer<T>::call)
, m_checker(&internal::function_traits_deducer<T>::check_args)
{
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