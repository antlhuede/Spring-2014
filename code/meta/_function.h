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

  arg_traits args[MAX_ARGS];
};

#define IMPLEMENT_FUNCTION_TRAITS_DEDUCER(CLASS_TYPE, IS_MEM_FUNC, IS_CONST)                  \
typedef CLASS_TYPE class_type;                                                                \
typedef typename std::conditional<std::is_same<R, void>::value, void_, R>::type return_type;  \
typedef std::tuple<Args...> arg_tuple;                                                        \
enum {                                                                                        \
  num_args = sizeof...(Args),                                                                 \
  has_return_value = (std::is_same<R, void>::value == false),                                 \
  is_const = IS_CONST,                                                                        \
  is_member_func = IS_MEM_FUNC,                                                               \
};                                                                                                                                \
                                                                                                                                  \
template <size_t> struct unwrap;                                                                                                  \
template <size_t N>                                                                                                               \
struct unwrap                                                                                                                     \
{                                                                                                                                 \
  static void args(arg_traits* args)                                                                                              \
  {                                                                                                                               \
    typedef std::tuple_element<sizeof...(Args)-N, arg_tuple>::type ElementType;                                                   \
    arg_traits& arg = args[sizeof...(Args)-N];                                                                                    \
    arg.type = &typeof<ElementType>();                                                                                            \
    arg.isPointer = std::is_pointer<ElementType>::value;                                                                          \
    arg.isReference = std::is_reference<ElementType>::value;                                                                      \
    arg.isConst = std::is_const<ElementType>::value;                                                                              \
    unwrap<N - 1>::args(args);                                                                                                    \
  }                                                                                                                               \
                                                                                                                                  \
  static bool check_args(const arg_traits* traits)                                                                                \
  {                                                                                                                               \
    typedef typename std::tuple_element<sizeof...(Args)-N, arg_tuple>::type ExpectedType;                                         \
    const type& arg_type = *traits[sizeof...(Args)-N].type;                                                                       \
    const type& expected_type = typeof<ExpectedType>();                                                                           \
    std::cout << "checking types - ArgType: " << arg_type.name() << "\tExpected Type: " << expected_type.name() << std::endl;     \
    if (arg_type != expected_type)                                                                                                \
      return false;                                                                                                               \
                                                                                                                                  \
    return unwrap<N - 1>::check_args(traits);                                                                                     \
  }                                                                                                                               \
                                                                                                                                  \
  template <class... ArgsT>                                                                                                       \
  static R call(base_function* function, const arg_traits* traits, void** args, ArgsT... parameters)                              \
  {                                                                                                                               \
    const int index = sizeof...(Args)-N;                                                                                          \
    typedef typename std::tuple_element<index, arg_tuple>::type ParameterType;                                                    \
    assert(traits[index].isReference == false);                                                                                   \
    unwrap<N - 1>::call(function, traits, args, std::forward<ArgsT>(parameters)..., *static_cast<ParameterType*>(args[index]));   \
  }                                                                                                                               \
};                                                                                                                                \
template <>                                                                                                                       \
struct unwrap<0>                                                                                                                  \
{                                                                                                                                 \
  static void args(arg_traits* args) { /*end recursion*/ }                                                                        \
                                                                                                                                  \
  static bool check_args(const arg_traits* traits) { return true; /*end recursion*/ }                                             \
  template <class... ArgsT>                                                                                                       \
  static R call(base_function* function, const arg_traits* traits, void** args, ArgsT... parameters)                              \
  {                                                                                                                               \
    typedef function_holder<R(*)(Args...)> FuncType;                                                                              \
    static_cast<FuncType*>(function)->function_ptr(std::forward<ArgsT>(parameters)...);                                           \
  }                                                                                                                               \
};                                                                                                                                \
                                                                                                                                  \
static void args(arg_traits* args)                                                                                                \
{                                                                                                                                 \
  unwrap<sizeof...(Args)>::args(args);                                                                                            \
}                                                                                                                                 \
                                                                                                                                  \
static void call(base_function* function, const arg_traits* traits, void** args)                                                  \
{                                                                                                                                 \
  assert((unwrap<sizeof...(Args)>::check_args(traits)));                                                                          \
  unwrap<sizeof...(Args)>::call(function, traits, args);                                                                          \
}

template <class T> struct function_traits_deducer;
template <class R, class... Args>
struct function_traits_deducer<R(*)(Args...)>
{
  typedef R(*function_type)(Args...);
  typedef nulltype class_type;
  typedef typename std::conditional<std::is_same<R, void>::value, void_, R>::type return_type;
  typedef std::tuple<Args...> arg_tuple;
  enum { 
    num_args = sizeof...(Args),
    has_return_value = (std::is_same<R, void>::value == false),
    is_const = false,
    is_member_func = false,
  };

  template <size_t> struct unwrap;
  template <size_t N>
  struct unwrap
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

    static bool check_args(const arg_traits* traits)
    {
      typedef typename std::tuple_element<sizeof...(Args)-N, arg_tuple>::type ExpectedType;
      const type& arg_type = *traits[sizeof...(Args)-N].type;
      const type& expected_type = typeof<ExpectedType>();
      std::cout << "checking types - ArgType: " << arg_type.name() << "\tExpected Type: " << expected_type.name() << std::endl;
      if (arg_type != expected_type)
        return false;
      
      return unwrap<N - 1>::check_args(traits);
    }

    template <class... ArgsT>
    static R call(base_function* function, const arg_traits* traits, void** args, ArgsT... parameters) 
    {
      const int index = sizeof...(Args)-N;
      typedef typename std::tuple_element<index, arg_tuple>::type ParameterType;
      if (traits[index].isReference)
        unwrap<N - 1>::call(function, traits, args, std::forward<ArgsT>(parameters)..., static_cast<ParameterType>(*static_cast<std::remove_reference<ParameterType>::type*>(args[index])));
      else
        unwrap<N - 1>::call(function, traits, args, std::forward<ArgsT>(parameters)..., *static_cast<std::remove_reference<ParameterType>::type*>(args[index]));
    }
  };
  template <>
  struct unwrap<0>
  {
    static void args(arg_traits* args) { /*end recursion*/ }

    static bool check_args(const arg_traits* traits) { return true; /*end recursion*/ }
    template <class... ArgsT>
    static R call(base_function* function, const arg_traits* traits, void** args, ArgsT... parameters)
    {
      typedef function_holder<R(*)(Args...)> FuncType;
      static_cast<FuncType*>(function)->function_ptr(std::forward<Args>(parameters)...);
    }
  };

  static void args(arg_traits* args)
  {
    unwrap<sizeof...(Args)>::args(args);
  }
    
  static void call(base_function* function, const arg_traits* traits, void** args)
  {
    assert((unwrap<sizeof...(Args)>::check_args(traits)));
    unwrap<sizeof...(Args)>::call(function, traits, args);
  }
};
template <class R, class U, class... Args>
struct function_traits_deducer<R(U::*)(Args...)>
{
  IMPLEMENT_FUNCTION_TRAITS_DEDUCER(U, true, false)
};
template <class R, class U, class... Args>
struct function_traits_deducer<R(U::*)(Args...)const>
{
  IMPLEMENT_FUNCTION_TRAITS_DEDUCER(U, true, true)
};


template <class T>
function_traits::function_traits(T func)
  : isMemberFunction(function_traits_deducer<T>::is_member_func)
  , isConst(function_traits_deducer<T>::is_const)
  , hasReturnValue(function_traits_deducer<T>::has_return_value)
  , classType(&typeof<function_traits_deducer<T>::class_type>())
  , returnType(&typeof<function_traits_deducer<T>::return_type>())
  , numArguments(function_traits_deducer<T>::num_args)
{
  function_traits_deducer<T>::args(args);
}
template <class T>
function_traits::function_traits(decl<T>)
  : isMemberFunction(function_traits_deducer<T>::is_member_func)
  , isConst(function_traits_deducer<T>::is_const)
  , hasReturnValue(function_traits_deducer<T>::has_return_value)
  , classType(&typeof<function_traits_deducer<T>::class_type>())
  , returnType(&typeof<function_traits_deducer<T>::return_type>())
  , numArguments(function_traits_deducer<T>::num_args)
{
  function_traits_deducer<T>::args(args);
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
    assert(sizeof...(Args) == m_traits.numArguments);
    assert(m_traits.hasReturnValue == false);
    const int size = sizeof...(args) + 1 ;
    void* args_ptr[size] = { &args... };
    m_caller(m_function, m_traits.args, args_ptr);
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
  typedef void(*Caller)(base_function* function, const arg_traits* traits, void** args);
  Caller m_caller = nullptr;
};

template <class T> function::function(T func, void* obj)
  : m_traits(func), m_initialized(true), m_object(obj)
  , m_function(new function_holder<T>(func))
  , m_caller(&function_traits_deducer<T>::call)
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