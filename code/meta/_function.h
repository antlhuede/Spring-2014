#pragma once

namespace meta {

struct base_function;
template <class T> struct function_holder;

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

  bool operator==(const function_traits& rhs) const;

  bool isMemberFunction = false;
  bool isConst = false;
  bool hasReturnValue = false;
  unsigned numArguments = 0;

  const type* classType = nullptr;
  const type* returnType = nullptr;

  arg_traits args[MAX_ARGS];
};

class function
{
public:
  function() = default;
  ~function();
  template <class T> function(T func, void* obj = nullptr);

  const function_traits& traits() const { return m_traits; }

  template <class... Args>
  void operator()(Args... args);

private:
  function_traits m_traits;
  bool m_initialized = false;
  void* m_object = nullptr;
  base_function* m_function = nullptr;
  typedef void(*Caller)(base_function* function, void* object, const arg_traits* traits, void** args);
  typedef bool(*ArgChecker)(const type** arg_types);

  Caller m_caller = nullptr;
  ArgChecker m_checker = nullptr;
};

namespace internal {
template <class T> struct caller;
template <class U, bool is_const, class R, class... Args> struct function_operator;
template <class U, bool is_const, class R, class... Args> struct base_deducer;
template <class R, class... Args> struct function_traits_deducer;
}
}
