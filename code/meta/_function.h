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
  function();
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

  template <class... Args>
  variant operator()(Args... args) const;

  template <class U, class... Args>
  variant Call(U* object, Args... args) const;

  template <class U, class... Args>
  variant Call(const U* object, Args... args) const;

  variant CallGeneric(const type* classType, const void* object, size_t numArgs, const type** argTypes, const void** args) const;
  variant CallGeneric(const type* classType, void* object, size_t numArgs, const type** argTypes, const void** args) const;

  typedef bool(*ArgChecker)(const type** argTypes, size_t numArgs);

  const function_traits traits;
  const bool initialized = false;
  ArgChecker const CheckArgs = nullptr;

private:
  typedef internal::base_function base_function;
  typedef variant(*Caller)(base_function* function, void* object, const arg_traits* traits, void** args);

  template <class T>
  void Construct(T func, void* object = nullptr);
  void* m_object = nullptr;
  base_function* m_function = nullptr;
  Caller m_caller = nullptr;
};
}
