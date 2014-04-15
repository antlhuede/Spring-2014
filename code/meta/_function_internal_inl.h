#pragma once

namespace meta { namespace internal {

template <class T> struct caller;
template <class T, class R, class... Args> struct function_operator;
template <class T, class U, class R, class... Args> struct base_deducer;
template <class T> struct function_traits_deducer;

//caller: used to correctly cast the base function pointer and call the function pointer
//lambda functions fall under the category of const member function
template <class R, class... Args>
struct caller<R(*)(Args...)>
{
  typedef function_holder<R(*)(Args...)> FuncType;
  static R Call(base_function* function, void* object, Args&&... parameters)
  {
    static_cast<FuncType*>(function)->function_ptr(std::forward<Args&&>(parameters)...);
  }
};
template <class R, class U, class... Args>
struct caller<R(U::*)(Args...)>
{
  typedef function_holder<R(U::*)(Args...)> FuncType;
  static R Call(base_function* function, void* object, Args&&... parameters)
  {
    (static_cast<U*>(object)->*(static_cast<FuncType*>(function)->function_ptr))(std::forward<Args&&>(parameters)...);
  }
};
template <class R, class U, class... Args>
struct caller<R(U::*)(Args...)const>
{
  typedef function_holder<R(U::*)(Args...)const> FuncType;
  static R Call(base_function* function, void* object, Args&&... parameters)
  {
    (static_cast<const U*>(object)->*(static_cast<FuncType*>(function)->function_ptr))(std::forward<Args&&>(parameters)...);
  }
};
//inherited class that gives function_traits_deducer abilities to operate
//on generic representations of data
template <class T, class R, class... Args>
struct function_operator
{
  template <size_t> struct unwrap;
  typedef typename unwrap<sizeof...(Args)> unwrapper;

  static void DeduceArgs(arg_traits* args)
  {
    unwrapper::DeduceArgs(args);
  }
  static bool CheckArgs(const type** arg_types)
  {
    return unwrapper::CheckArgs(arg_types);
  }
  static void Call(base_function* function, void* object, const arg_traits* traits, void** args)
  {
    unwrapper::Call(function, object, traits, args);
  }

  typedef std::tuple<Args...> arg_tuple;
  typedef T function_type;

  //template recursion to:
  //  deduce function arguments,
  //  check passed argument types
  //  forward a function call
  template <size_t N> struct unwrap
  {
    static void DeduceArgs(arg_traits* args)
    {
      typedef std::tuple_element<sizeof...(Args)-N, arg_tuple>::type ElementType;
      arg_traits& arg = args[sizeof...(Args)-N];
      arg.type = typeof<ElementType>();
      arg.isPointer = std::is_pointer<ElementType>::value;
      arg.isReference = std::is_reference<ElementType>::value;
      arg.isConst = std::is_const<ElementType>::value;
      unwrap<N - 1>::DeduceArgs(args);
    }
    static bool CheckArgs(const type** arg_types)
    {
      typedef typename std::tuple_element<sizeof...(Args)-N, arg_tuple>::type ExpectedType;
      const type* arg_type = arg_types[sizeof...(Args)-N];
      const type* expected_type = typeof<ExpectedType>();
      if (*arg_type != *expected_type)
        return false;
      return unwrap<N - 1>::CheckArgs(arg_types);
    }
    template <class... ArgsT>
    static R Call(base_function* function, void* object, const arg_traits* traits, void** args, ArgsT&&... parameters)
    {
      //remove reference so we can have a pointer to the memory, then cast the reference back if it existed.
      //this results in doing absolutely nothing to things that arent a reference
      //results in references being passed in as their correct type so we dont get corrupted stack memory
      //  static_cast<ParameterType>(*static_cast<std::remove_reference<ParameterType>::type*>(memory))
      typedef typename std::tuple_element<sizeof...(Args)-N, arg_tuple>::type ParameterType;
      unwrap<N - 1>::Call(function, object, traits, args, std::forward<ArgsT&&>(parameters)...,
        static_cast<ParameterType>(*static_cast<std::remove_reference<ParameterType>::type*>(args[sizeof...(Args)-N])));
    }
  };
  template <> struct unwrap<0>
  {
    static void DeduceArgs(arg_traits* args) { /*end recursion*/ }
    static bool CheckArgs(const type** arg_types) { return true; /*end recursion*/ }
    template <class... ArgsT>
    static R Call(base_function* function, void* object, const arg_traits* traits, void** args, ArgsT&&... parameters)
    {
      caller<function_type>::Call(function, object, std::forward<Args&&>(parameters)...);
    }
  };
};
//deductions that are common among all function pointers
template <class T, class U, class R, class... Args>
struct base_deducer : public function_operator<T, R, Args...>
{
  enum {
    num_args = sizeof...(Args),
    has_return_value = (std::is_same<R, void>::value == false),
    is_member_func = (std::is_same<U, nulltype>::value == false),
  };

  typedef T function_type;
  typedef U class_type;
  typedef typename std::conditional<has_return_value, R, void_>::type return_type;
};

//catches all types, however only valid for types that implement operator() (functors)
//is the fallback option for the compiler and therefore handles lambda functions
template <class T>
struct function_traits_deducer : function_traits_deducer<decltype(&T::operator())>
{
  typedef typename nulltype class_type;
  typedef typename base_deducer::return_type return_type;
  typedef typename base_deducer::function_type function_type;
  enum { is_lambda = true, is_const = true };
};

//global functions
template <class R, class... Args>
struct function_traits_deducer<R(*)(Args...)> : public base_deducer<R(*)(Args...), nulltype, R, Args...>
{
  typedef typename base_deducer::class_type class_type;
  typedef typename base_deducer::return_type return_type;
  typedef typename base_deducer::function_type function_type;
  enum { is_lambda = false, is_const = false };
};
//non-const member functions
template <class U, class R, class... Args>
struct function_traits_deducer<R(U::*)(Args...)> : public base_deducer<R(U::*)(Args...), U, R, Args...>
{
  typedef typename base_deducer::class_type class_type;
  typedef typename base_deducer::return_type return_type;
  typedef typename base_deducer::function_type function_type;
  enum { is_lambda = false, is_const = false };
};
//const member functions
template <class U, class R, class... Args>
struct function_traits_deducer<R(U::*)(Args...)const> : public base_deducer<R(U::*)(Args...)const, U, R, Args...>
{
  typedef typename base_deducer::class_type class_type;
  typedef typename base_deducer::return_type return_type;
  typedef typename base_deducer::function_type function_type;
  enum { is_lambda = false, is_const = true };
};
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
}}