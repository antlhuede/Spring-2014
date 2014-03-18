#pragma once

namespace meta
{
template <class T>
list<T>::list()
{
  // Push back onto the list
  m_pNext = head();
  head() = (T*)(this);
}

template <class T>
T* list<T>::next()
{
  return m_pNext;
}

template <class T>
const T* list<T>::next() const
{
  return m_pNext;
}

template <class T>
T*& list<T>::head()
{
  static T* head = 0;
  return head;
}
}