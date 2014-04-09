#pragma once

namespace meta { namespace internal {
template <class R, class... Args>
struct caller<R(*)(Args...)>
{
  typedef function_holder<R(*)(Args...)> FuncType;
  template <class... Args>
  static R call(base_function* function, void* object, Args... parameters)
  {
    static_cast<FuncType*>(function)->function_ptr(std::forward<Args>(parameters)...);
  }
};
template <class R, class U, class... Args>
struct caller<R(U::*)(Args...)>
{
  typedef function_holder<R(U::*)(Args...)> FuncType;
  template <class... Args>
  static R call(base_function* function, void* object, Args... parameters)
  {
    (static_cast<U*>(object)->*(static_cast<FuncType*>(function)->function_ptr))(std::forward<Args>(parameters)...);
  }
};
template <class R, class U, class... Args>
struct caller<R(U::*)(Args...)const>
{
  typedef function_holder<R(U::*)(Args...)const> FuncType;
  template <class... Args>
  static R call(base_function* function, void* object, Args... parameters)
  {
    (static_cast<const U*>(object)->*(static_cast<FuncType*>(function)->function_ptr))(std::forward<Args>(parameters)...);
  }
};

template <class T, class U, class R, class... Args>
struct function_operator
{
  template <size_t> struct unwrap;
  typedef typename unwrap<sizeof...(Args)> unwrapper;

  static void deduce_args(arg_traits* args)
  {
    unwrapper::deduce_args(args);
  }
  static bool check_args(const type** arg_types)
  {
    return unwrapper::check_args(arg_types);
  }
  static void call(base_function* function, void* object, const arg_traits* traits, void** args)
  {
    unwrapper::call(function, object, traits, args);
  }

  typedef std::tuple<Args...> arg_tuple;
  typedef T FuncType;

  template <size_t N> struct unwrap
  {
    static void deduce_args(arg_traits* args)
    {
      typedef std::tuple_element<sizeof...(Args)-N, arg_tuple>::type ElementType;
      arg_traits& arg = args[sizeof...(Args)-N];
      arg.type = &typeof<ElementType>();
      arg.isPointer = std::is_pointer<ElementType>::value;
      arg.isReference = std::is_reference<ElementType>::value;
      arg.isConst = std::is_const<ElementType>::value;
      unwrap<N - 1>::deduce_args(args);
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
    static R call(base_function* function, void* object, const arg_traits* traits, void** args, ArgsT... parameters)
    {
      const int index = sizeof...(Args)-N;
      typedef typename std::tuple_element<index, arg_tuple>::type ParameterType;

        unwrap<N - 1>::call(function, object, traits, args, std::forward<ArgsT>(parameters)...,
          static_cast<ParameterType>(*static_cast<std::remove_reference<ParameterType>::type*>(args[index])));

    }
  };
  template <> struct unwrap<0>
  {
    static void deduce_args(arg_traits* args) { /*end recursion*/ }
    static bool check_args(const type** arg_types) { return true; /*end recursion*/ }
    template <class... ArgsT>
    static R call(base_function* function, void* object, const arg_traits* traits, void** args, ArgsT... parameters)
    {
      caller<FuncType>::call(function, object, std::forward<Args>(parameters)...);
    }
  };
};

template <class T, class U, class R, class... Args>
struct base_deducer : public function_operator<T, U, R, Args...>
{
  enum {
    num_args = sizeof...(Args),
    has_return_value = (std::is_same<R, void>::value == false),
    is_member_func = (std::is_same<U, nulltype>::value == false),
  };

  typedef T func_type;
  typedef U class_type;
  typedef typename std::conditional<has_return_value, R, void_>::type return_type;
};

template <class T>
struct function_traits_deducer : function_traits_deducer<decltype(&T::operator())>
{
  typedef typename nulltype class_type;
  typedef typename base_deducer::return_type return_type;
  typedef typename base_deducer::FuncType function_type;
  enum { is_lambda = true, is_const = true };
};
template <class R, class... Args>
struct function_traits_deducer<R(*)(Args...)> : public base_deducer<R(*)(Args...), nulltype, R, Args...>
{
  typedef typename base_deducer::class_type class_type;
  typedef typename base_deducer::return_type return_type;
  typedef typename base_deducer::FuncType function_type;
  enum { is_lambda = false, is_const = false };
};
template <class U, class R, class... Args>
struct function_traits_deducer<R(U::*)(Args...)> : public base_deducer<R(U::*)(Args...), U, R, Args...>
{
  typedef typename base_deducer::class_type class_type;
  typedef typename base_deducer::return_type return_type;
  typedef typename base_deducer::FuncType function_type;
  enum { is_lambda = false, is_const = false };
};
template <class U, class R, class... Args>
struct function_traits_deducer<R(U::*)(Args...)const> : public base_deducer<R(U::*)(Args...)const, U, R, Args...>
{
  typedef typename base_deducer::class_type class_type;
  typedef typename base_deducer::return_type return_type;
  typedef typename base_deducer::FuncType function_type;
  enum { is_lambda = false, is_const = true };
};
}}