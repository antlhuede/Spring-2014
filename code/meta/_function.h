#pragma once

namespace meta { 

namespace internal {
  struct base_function;
  template <class T> struct function_holder;
  template <class T> struct signature;
}

//describes an argument to a function
struct arg_traits
{
  bool isReference = false;
  bool isPointer = false;
  bool isConst = false;
  const type* type = nullptr;
};

//stores information about function pointers
struct function_traits
{
  static const unsigned MAX_ARGS = 10;

  function_traits() = default;

  template <class T>
  function_traits(T func);

  bool operator==(const function_traits& rhs) const;
  bool operator!=(const function_traits& rhs) const;

  bool isMemberFunction = false;
  bool isConst = false;
  bool hasReturnValue = false;
  bool isLambda = false;
  unsigned numArguments = 0;

  const type* classType = nullptr;
  const type* returnType = nullptr;

  arg_traits args[MAX_ARGS];
};

//generic function object able to store and call pointers to any function
class function
{
public:
  function() = default;
  ~function();
  function(function&& other);
  function(const function& other);

  template <class T> 
  function(T lambda);

  template <class R, class... Args>
  function(R(*func)(Args...));
  template <class R, class U, class... Args>
  function(R(U::*func)(Args...), U* obj = nullptr);
  template <class R, class U, class... Args>
  function(R(U::*func)(Args...)const, const U* obj = nullptr);
  
  const function_traits& traits() const { return m_traits; }

  template <class... Args>
  variant operator()(Args... args) const;

  template <class U, class... Args>
  variant call(U* object, Args... args) const;

  template <class U, class... Args>
  variant call(const U* object, Args... args) const;

  template <class U, class... Args>
  bool check_types() const;
private:
  template <class T>
  void construct(T func);
  typedef internal::base_function base_function;
  function_traits m_traits;
  bool m_initialized = false;
  void* m_object = nullptr;
  base_function* m_function = nullptr;
  typedef variant(*Caller)(base_function* function, void* object, const arg_traits* traits, void** args);
  typedef bool(*ArgChecker)(const type** arg_types);

  Caller m_caller = nullptr;
  ArgChecker m_checker = nullptr;
};
}
