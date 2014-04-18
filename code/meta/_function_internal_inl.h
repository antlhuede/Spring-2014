#pragma once

#include "_function_caller.h"

namespace meta { namespace internal {

//so we can have a handle to a function
struct base_function { virtual ~base_function() {} virtual base_function* clone() const = 0; };
//so we can store the function as its correct type
template <class T>
struct function_holder : public base_function
{
  function_holder(T function_) : function_ptr(function_) {}
  //clone is to allow copy construction of functions, 
  //otherwise we need to know the actual type of underlying function of rhs object, which is impossible.
  virtual base_function* clone() const { return new function_holder<T>(function_ptr); }
  T function_ptr;
};

template <class T, class R, class... Args> struct function_operator;
template <class T, class U, class R, class... Args> struct base_deducer;
template <class T> struct function_traits_deducer;

//inherited class that gives function_traits_deducer abilities to operate
//on generic representations of data
template <class T, class R, class... Args>
struct function_operator
{
  template <size_t> struct unwrap;
  static const int NUM_ARGS = sizeof...(Args);
  typedef typename unwrap<sizeof...(Args)> unwrapper;
  typedef std::tuple<Args...> arg_tuple;
  typedef T function_type;

  static inline void DeduceArgs(arg_traits* args)
  {
    unwrapper::DeduceArgs(args);
  }
  static inline bool CheckArgs(const type** argTypes)
  {
    return unwrapper::CheckArgs(argTypes);
  }
  static inline variant Call(base_function* function, void* object, const arg_traits* traits, void** args)
  {
    return variant(unwrapper::Call(function, object, traits, args));
  }

  //template recursion to:
  //  deduce function arguments,
  //  check passed argument types
  //  forward a function call
  template <size_t N> struct unwrap
  {
    static inline void DeduceArgs(arg_traits* args)
    {
      typedef std::tuple_element<sizeof...(Args)-N, arg_tuple>::type ElementType;
      arg_traits& arg = args[sizeof...(Args)-N];
      arg.type = typeof<ElementType>();
      arg.isPointer = std::is_pointer<ElementType>::value;
      arg.isReference = std::is_reference<ElementType>::value;
      arg.isConst = std::is_const<ElementType>::value;
      unwrap<N - 1>::DeduceArgs(args);
    }
    static inline bool CheckArgs(const type** arg_types)
    {
      typedef typename std::tuple_element<sizeof...(Args)-N, arg_tuple>::type ExpectedType;
      const type* arg_type = arg_types[sizeof...(Args)-N];
      const type* expected_type = typeof<ExpectedType>();
      if (*arg_type != *expected_type)
        return false;
      return unwrap<N - 1>::CheckArgs(arg_types);
    }
    template <class... ArgsT>
    static inline R Call(base_function* function, void* object, const arg_traits* traits, void** args, ArgsT&&... parameters)
    {
      //remove reference so we can have a pointer to the memory, then cast the reference back if it existed.
      //this results in doing absolutely nothing to things that arent a reference
      //results in references being passed in as their correct type so we dont get corrupted stack memory
      //  static_cast<ParameterType>(*static_cast<std::remove_reference<ParameterType>::type*>(memory))
      typedef typename std::tuple_element<sizeof...(Args)-N, arg_tuple>::type ParameterType;
      return unwrap<N - 1>::Call(function, object, traits, args, std::forward<ArgsT&&>(parameters)...,
        static_cast<ParameterType>(*static_cast<std::remove_reference<ParameterType>::type*>(args[sizeof...(Args)-N])));
    }
  };
  template <> struct unwrap<0>
  {
    static inline void DeduceArgs(arg_traits* args) { /*end recursion*/ }
    static inline bool CheckArgs(const type** arg_types) { return true; /*end recursion*/ }
    template <class... ArgsT>
    static inline R Call(base_function* function, void* object, const arg_traits* traits, void** args, ArgsT&&... parameters)
    {
      return caller<R, function_type>::Call(function, object, std::forward<Args>(parameters)...);
    }
  };
};

template <class T> struct check_return
{
  enum { is_void = false };
  typedef T type;
};

template <> struct check_return<void>
{
  enum { is_void = true };
  typedef void_ type;
};

template <class T> struct function_deducer;

template <class R, class... Args>
struct function_deducer<R(*)(Args...)>
{
  typedef R(*func_type)(Args...);
  typedef typename check_return<R>::type return_type;
  typedef nulltype class_type;
  typedef function_operator<func_type, return_type, Args...> func_operator;
  enum { num_args = sizeof...(Args), is_member_func = false, is_const = false, is_lambda = false };

  static inline base_function* Create(func_type func) { return new function_holder<func_type>(func); }
};
template <class R, class U, class... Args>
struct function_deducer<R(U::*)(Args...)>
{
  typedef R(U::*func_type)(Args...);
  typedef typename check_return<R>::type return_type;
  typedef U class_type;
  typedef function_operator<func_type, return_type, Args...> func_operator;
  enum { num_args = sizeof...(Args), is_member_func = true, is_const = false, is_lambda = false };

  static inline base_function* Create(func_type func) { return new function_holder<func_type>(func); }
};
template <class R, class U, class... Args>
struct function_deducer<R(U::*)(Args...)const>
{
  typedef R(U::*func_type)(Args...)const;
  typedef typename check_return<R>::type return_type;
  typedef U class_type;
  typedef function_operator<func_type, return_type, Args...> func_operator;
  enum { num_args = sizeof...(Args), is_member_func = true, is_const = true, is_lambda = false };

  static inline base_function* Create(func_type func) { return new function_holder<func_type>(func); }
};

//lambda deducer
template <class S> 
struct function_deducer
{
  typedef decltype(&S::operator()) func_type;
  typedef typename function_deducer<func_type>::return_type return_type;
  typedef nulltype class_type;
  typedef typename function_deducer<func_type>::func_operator func_operator;

  enum { num_args = function_deducer<func_type>::num_args, is_member_func = false, is_const = true, is_lambda = true };
  static inline base_function* Create(S lambda) { return new function_holder<func_type>(&S::operator()); }
};

template <class T>
struct function_descriptor : public function_deducer<T>
{
public:
  enum {
    num_args = function_deducer<T>::num_args,
    is_member_function = function_deducer<T>::is_member_func,
    is_const = function_deducer<T>::is_const,
    is_lambda = function_deducer<T>::is_lambda,
    has_return_value = (std::is_same<return_type, void_>::value == false),
  };

  typedef typename function_deducer<T>::class_type class_type;
  typedef typename function_deducer<T>::return_type return_type;
  typedef typename function_deducer<T>::func_type func_type;
  typedef typename function_deducer<T>::func_operator func_operator;

  static inline void DeduceArgs(arg_traits* args)
  {
    func_operator::DeduceArgs(args);
  }
  static inline bool CheckArgs(const type** argTypes, size_t numArgs)
  {
    if (numArgs != num_args) return false;
    return func_operator::CheckArgs(argTypes);
  }
  static inline variant Call(base_function* function, void* object, const arg_traits* traits, void** args)
  {
    return func_operator::Call(function, object, traits, args);
  }
  static inline base_function* Create(T func) { return function_deducer<T>::Create(func); }
};
}}