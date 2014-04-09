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
  typedef void(*Caller)(base_function* function, void* object, const arg_traits* traits, void** args);
  typedef bool(*ArgChecker)(const type** arg_types);

  Caller m_caller = nullptr;
  ArgChecker m_checker = nullptr;
};
}
