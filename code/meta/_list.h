#pragma once

namespace meta
{
template <class T>
class list
{
public:
  list();

  T* next();
  const T* next() const;

  static T*& head();
private:
  T* m_pNext;
};
}
