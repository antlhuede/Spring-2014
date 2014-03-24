#pragma once

namespace meta {

class function_traits
{
public:
  static const unsigned MAX_ARGS = 10;

  function_traits();

  template <class R, class... Args>
  function_traits(R(*func)(Args... args));

  template <class U, class R, class... Args>
  function_traits(R(U::*func)(Args... args), void* obj);

  template <class U, class R, class... Args>
  function_traits(R(U::*func)(Args... args)const, const void* obj);

private:
  bool m_isMemberFunction = false;
  bool m_isConst = false;
  bool m_hasReturnValue = false;
  unsigned m_numArguments;

  const type* m_classType = nullptr;
  const type* m_returnType = nullptr;
  const type* m_argTypes[MAX_ARGS];


};
class function
{
public:
  function();
  template <class T> function(T func, void* obj = nullptr);
  template <class T> function(T func, const void* obj = nullptr);


private:
};
}