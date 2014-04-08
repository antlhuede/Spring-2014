#pragma once

namespace meta {

struct base_function { virtual ~base_function() {} };

template <class T>
struct function_holder : public base_function {
  function_holder(T function_) : function_ptr(function_) {}
  T function_ptr;
};

struct arg_traits
{
  bool isReference = false;
  bool isPointer = false;
  bool isConst = false;
  const type* type = nullptr;
};

struct function_traits
{
  static const unsigned MAX_ARGS = 10;

  function_traits() = default;

  template <class T>
  function_traits(T func);
  template <class T>
  function_traits(decl<T>);

  bool operator==(const function_traits& rhs) const;

  bool isMemberFunction = false;
  bool isConst = false;
  bool hasReturnValue = false;
  unsigned numArguments = 0;

  const type* classType = nullptr;
  const type* returnType = nullptr;

  arg_traits args[MAX_ARGS];
};

namespace internal {
template <class R, class U, bool is_const, class... Args>
struct function_operator;
template <class T> struct function_traits_deducer;
}

class function
{
public:
  function() = default;
  ~function() { if (m_function) delete m_function; }
  template <class T> function(T func, void* obj = nullptr);

  const function_traits& traits() const { return m_traits; }

  template <class... Args>
  void operator()(Args... args);

private:
  function_traits m_traits;
  bool m_initialized = false;
  void* m_object = nullptr;
  base_function* m_function = nullptr;
  typedef void(*Caller)(base_function* function, const arg_traits* traits, void** args);
  typedef bool(*ArgChecker)(const type** arg_types);

  Caller m_caller = nullptr;
  ArgChecker m_checker = nullptr;
};
}


//template <class R, class... Args>
//function_traits::function_traits(R(*func)(Args...))
//  : isMemberFunction(false)
//  , isConst(false)
//  , hasReturnValue(std::is_same<R, void>::value == false)
//  , numArguments(sizeof...(Args))
//  , classType(nullptr)
//  , returnType(nullptr)
//{
//  assert(sizeof...(Args) <= MAX_ARGS);
//  unwrapper::argTypes(func, argTypes);
//}

//template <class R, class U, class... Args>
//function_traits::function_traits(R(U::*func)(Args...))
//  : isMemberFunction(true)
//  , isConst(false)
//  , hasReturnValue(std::is_same<R, void>::value == false)
//  , numArguments(sizeof...(Args))
//  , classType(&typeof<U>())
//  , returnType(nullptr)
//{
//}
//template <class R, class U, class... Args>
//function_traits::function_traits(R(U::*func)(Args...)const)
//{
//  assert(sizeof...(Args) <= MAX_ARGS);
//  unwrapper::argTypes(func, argTypes);
//}


//
//template <size_t N>
//struct unwrap
//{
//template <class R, class... Args>
//static void argTypes(R(*func)(Args... args), const type** v)
//{
//  typedef std::tuple<Args...> arg_tuple;
//  v[sizeof...(Args)-N] = &typeof<std::tuple_element<sizeof...(Args)-N, arg_tuple>::type>();
//  unwrap<N - 1>::argTypes(func, v);
//}
//template <class R, class U, class... Args>
//static void argTypes(R(U::*func)(Args... args), const type** v)
//{
//  typedef std::tuple<Args...> arg_tuple;
//  v[sizeof...(Args)-N] = &typeof<std::tuple_element<sizeof...(Args)-N, arg_tuple>::type>();
//  unwrap<N - 1>::argTypes(func, v);
//}
//template <class R, class... Args>
//static R call(R(*func)(Args...), const type** t, void** v)
//{
//  typedef std::tuple<Args...> arg_tuple;
//  assert(*v[sizeof...(Args)-N] == typeof<std::tuple_element<sizeof...(Args)-N, arg_tuple>::type>());
//  return unwrap<N - 1>::call(func, t, v);
//}
//};
//template <>
//struct unwrap<0>
//{
//template <class R, class... Args>
//static void argTypes(R(*func)(Args...), const type** v) { /* ends recursion */ }
//template <class R, class U, class... Args>
//static void argTypes(R(U::*func)(Args...), const type** v) { /* ends recursion */ }
//template <class R, class... Args, class... ArgsT>
//static R call(R(*func)(Args...), Ar)
//{
//  return func(args...);
//}
//};



//struct unwrapper
//{
//template <class R, class... Args>
//static void argTypes(R(*func)(Args...), const type** v, unsigned* num_args) 
//{
//  assert(sizeof...(Args) <= function_traits::MAX_ARGS); 
//  assert(num_args);
//  *num_args = sizeof...(Args);
//  unwrap<sizeof...(Args)>::argTypes(func, v); 
//}
//template <class R, class U, class... Args>
//static void argTypes(R(U::*func)(Args...), const type** v, unsigned* num_args)
//{
//  assert(sizeof...(Args) <= function_traits::MAX_ARGS);
//  assert(num_args);
//  *num_args = sizeof...(Args);
//  unwrap<sizeof...(Args)>::argTypes(func, v);
//}
//template <class R, class... Args>
//static R call(R(*func)(Args...), const type** t, void** v) { }
//template <class R, class U, class... Args>
//static R call(R(U::*func)(Args...), const type** t, void** v) { }
//};