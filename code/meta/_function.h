#pragma once

namespace meta {

struct base_function { virtual ~base_function() {} };

template <class T>
struct function_holder : public base_function {
  function_holder(T function_) : function_ptr(function_) {}
  T function_ptr;
};

class void_ {};


struct function_traits
{
  static const unsigned MAX_ARGS = 10;

  function_traits() = default;

  template <class T>
  function_traits(T func);

  //template <class R, class... Args>
  //function_traits(R(*func)(Args...));
  //
  //template <class R, class U, class... Args>
  //function_traits(R(U::*func)(Args...));
  //
  //template <class R, class U, class... Args>
  //function_traits(R(U::*func)(Args...)const);

  bool operator==(const function_traits& rhs) const
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
      if (*argTypes[i] != *rhs.argTypes[i])
        return false;

    return true;
  }
  bool isMemberFunction = false;
  bool isConst = false;
  bool hasReturnValue = false;
  unsigned numArguments = 0;

  const type* classType = nullptr;
  const type* returnType = nullptr;
  const type* argTypes[MAX_ARGS];
};

template <class T> struct function_traits_deducer;
template <class R, class... Args>
struct function_traits_deducer<R(*)(Args...)>
{
  typedef nulltype class_type;
  typedef R return_type;
  enum {
    num_args = sizeof...(Args),
    has_return_value = (std::is_same<R, void>::value == false),
    is_const = false,
    is_member_func = false,
  };
};
template <class R, class U, class... Args>
struct function_traits_deducer<R(U::*)(Args...)>
{
  typedef U class_type;
  typedef R return_type;
  enum {
    num_args = sizeof...(Args),
    has_return_value = (std::is_same<R, void>::value == false),
    is_const = false,
    is_member_func = true,
  };
};


template <size_t N>
struct unwrap
{
template <class R, class... Args>
static void argTypes(R(*func)(Args... args), const type** v)
{
  typedef std::tuple<Args...> arg_tuple;
  v[sizeof...(Args)-N] = &typeof<std::tuple_element<sizeof...(Args)-N, arg_tuple>::type>();
  unwrap<N - 1>::argTypes(func, v);
}
template <class R, class U, class... Args>
static void argTypes(R(U::*func)(Args... args), const type** v)
{
  typedef std::tuple<Args...> arg_tuple;
  v[sizeof...(Args)-N] = &typeof<std::tuple_element<sizeof...(Args)-N, arg_tuple>::type>();
  unwrap<N - 1>::argTypes(func, v);
}
//template <class R, class... Args>
//static R call(R(*func)(Args...), const type** t, void** v)
//{
//  typedef std::tuple<Args...> arg_tuple;
//  assert(*v[sizeof...(Args)-N] == typeof<std::tuple_element<sizeof...(Args)-N, arg_tuple>::type>());
//  return unwrap<N - 1>::call(func, t, v);
//}
};
template <>
struct unwrap<0>
{
template <class R, class... Args>
static void argTypes(R(*func)(Args...), const type** v) { /* ends recursion */ }
template <class R, class U, class... Args>
static void argTypes(R(U::*func)(Args...), const type** v) { /* ends recursion */ }
//template <class R, class... Args, class... ArgsT>
//static R call(R(*func)(Args...), Ar)
//{
//  return func(args...);
//}
};

struct unwrapper
{
template <class R, class... Args>
static void argTypes(R(*func)(Args...), const type** v, unsigned* num_args) 
{
  assert(sizeof...(Args) <= function_traits::MAX_ARGS); 
  assert(num_args);
  *num_args = sizeof...(Args);
  unwrap<sizeof...(Args)>::argTypes(func, v); 
}
template <class R, class U, class... Args>
static void argTypes(R(U::*func)(Args...), const type** v, unsigned* num_args)
{
  assert(sizeof...(Args) <= function_traits::MAX_ARGS);
  assert(num_args);
  *num_args = sizeof...(Args);
  unwrap<sizeof...(Args)>::argTypes(func, v);
}
template <class R, class... Args>
static R call(R(*func)(Args...), const type** t, void** v) { }
template <class R, class U, class... Args>
static R call(R(U::*func)(Args...), const type** t, void** v) { }
};

template <class... Args>
static void function_impl(Args... args) {}
//template <class R, class... Args>
//static R function_impl(Args... args) { return R(); }

template <class T>
function_traits::function_traits(T func)
  //: isMemberFunction(std::is_member_function_pointer<T>::value)
  //, isConst(std::is_const<T>::value)
  //, hasReturnValue(std::is_same<std::function<decltype(func)>::result_type, void>::value == false)
  //, classType(&typeof<std::function<T>::target_type>>())
  //, returnType(&typeof<decltype(func())>())
  : isMemberFunction(function_traits_deducer<T>::is_member_func)
{
  unwrapper::argTypes(func, argTypes, &numArguments);
}

class function
{
public:
  function() = default;
  ~function() { if (m_function) delete m_function; }
  template <class T> function(T func, void* obj = nullptr);

  const function_traits& traits() const { return m_traits; }

  template <class... Args>
  void operator()(Args... args)
  {
    typedef void(*FuncType)(Args...);
    function_traits traits(&function_impl<Args...>);
    assert(traits == m_traits);
    reinterpret_cast<function_holder<FuncType>*>(m_function)->function_ptr(args...);
  }
  //void operator()()
  //{
  //  typedef void(*FuncType)();
  //  FuncType func = nullptr;
  //  function_traits traits(func);
  //  assert(m_traits == traits);
  //  reinterpret_cast<function_holder<FuncType>*>(m_function)->function_ptr();
  //}
private:
  function_traits m_traits;
  bool m_initialized = false;
  void* m_object = nullptr;
  base_function* m_function = nullptr;
};

template <class T> function::function(T func, void* obj)
  : m_traits(func), m_initialized(true), m_object(obj)
  , m_function(new function_holder<T>(func))
{
}
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

