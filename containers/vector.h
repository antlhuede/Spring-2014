#pragma once

namespace alib
{
namespace containers
{
template <class T>
class vector
{
public:
  typedef T value_type;

  class iterator {};
  class const_iterator {};

  vector();
  vector(size_t num_elems);
  vector(initializer_list<T>);

  vector(const vector&);
  vector& operator=(const vector&);

  vector(vector&&);
  vector& operator=(vector&&);

  //iterators
  iterator begin();
  const_iterator begin() const;
  iterator end();
  const_iterator end() const;

  iterator rbegin();
  const_iterator rbegin() const;
  iterator rend();
  const_iterator rend() const;

  const_iterator cbegin() const;
  const_iterator cend() const;

  const_iterator crbegin() const;
  const_iterator crend() const;

  //capacity
  size_t size() const;
  size_t max_size() const;

  //set a default value to construct nodes to... adds to overhead
  //however allows for construction of objects on return of memory to user
  void resize(size_t n, value_type val = value_type());
  size_t capacity() const;
  size_t empty() const;
  void reserve(size_t n);
  
  //element access
  value_type& operator[](size_t index);
  const value_type& operator[](size_t index) const;
  value_type& at(size_t index);
  const value_type& at(size_t index) const;

  value_type& front();
  const value_type& front() const;
  value_type& back();
  const value_type& back() const;
  shared_ptr<value_type> data() noexcept { return m_data; }
  const shared_ptr<value_type>& data() const noexcept { return m_data; }

  //modifiers
  template <class InputIterator> 
  void assign(InputIterator first, InputIterator last);
  void assign(size_t n, const value_type& val);
  void assign(initializer_list<value_type> list);

  void push_back(const value_type& val);
  void push_back(value_type&& val);
  void pop_back();

  template <class InputIterator>
  iterator insert(const_iterator position, InputIterator first, InputIterator last);
  iterator insert(const_iterator position, const value_type& val);
  iterator insert(const_iterator position, size_t n, const value_type& val);
  iterator insert(const_iterator position, value_type&& val);
  iterator insert(const_iterator position, initializer_list<value_type> list);

  iterator erase(const_iterator position);
  iterator erase(const_iterator first, const_iterator last);

  void swap(vector& x);
  void clear() noexcept;
  
  template <class... Args>
  iterator emplace(const_iterator position, Args&&... args);
  template <class... Args>
  void emplace_back(Args&&... args);
private:
  void grow();

  shared_ptr<value_type> m_data;
  size_t m_size;
  size_t m_capacity;

  //these are powers of two because my growth strategy is in powers of two
  static const size_t max_elements = 1024; //arbitrary, set higher if necessary
  static const size_t default_size = 16; //arbitrary, set higher/lower if necessary
};

#include <cassert>

inline size_t get_next_power_of_two(size_t value)
{
  size_t pow = 2;
  while (pow <= value) pow <<= 1;
  return pow;
}

template <typename T>
shared_ptr<T> allocate_memory(size_t num_elements)
{
  size_t size = sizeof(T)* num_elements;
  shared_ptr<T> ret((T*)(new char[size]));
  std::memset(ret.get(), 0, size);
  return ret;
}

template <class T>
void vector<T>::grow()
{
  assert(m_size == m_capacity);
  m_capacity = get_next_power_of_two(m_capacity);
  shared_ptr<T> new_data(allocate_memory<T>(m_capacity));
  T* new_mem = new_data.get();
  T* old_mem = m_data.get();

  for (size_t i = 0; i < m_size; ++i)
    *new_mem++ = std::move(*old_mem++);

  m_data.swap(new_data);
}
template <class T>
vector<T>::vector()
  : m_size(0)
  , m_capacity(default_size)
  , m_data(allocate_memory<T>(default_size)) {}

template <class T>
vector<T>::vector(size_t num_elems)
  : m_size(0)
  , m_capacity(get_next_power_of_two(num_elems))
{
  m_data = allocate_memory<T>(m_capacity);
}

template <class T>
vector<T>::vector(initializer_list<T> list)
  : m_size(list.size())
{
  m_capacity = m_size > default_size ? get_next_power_of_two(m_size) : default_size;
  m_data = allocate_memory<T>(m_capacity);
  T* v = m_data.get();
  for (auto it = list.begin(); it != list.end(); ++it)
    new (v++) T(*it);
}


//capacity
template <class T>
size_t vector<T>::capacity() const { return m_capacity; }
template <class T>
size_t vector<T>::size() const { return m_size; }
template <class T>
size_t vector<T>::max_size() const { return max_elements; }

//element access
template <class T>
T& vector<T>::operator[](size_t index)
{
  return m_data.get()[index];
}
template <class T>
const T& vector<T>::operator[](size_t index) const
{
  return m_data.get()[index];
}
template <class T>
T& vector<T>::at(size_t index)
{
  assert(false); //not properly implemented
  return m_data.get()[index];
}
template <class T>
const T& vector<T>::at(size_t index) const
{
  assert(false); //not properly implemented
  return m_data.get()[index];
}

template <class T>
T& vector<T>::front()
{
  assert(m_size);
  return (*this)[0];
}

template <class T>
const T& vector<T>::front() const
{
  return const_cast<vector<T>*>(this)->front();
}

template <class T>
T& vector<T>::back()
{
  assert(m_size);
  return (*this)[m_size - 1];
}

template <class T>
const T& vector<T>::back() const
{
  return const_cast<vector<T>*>(this)->back();
}

//insertion
template <class T>
void vector<T>::push_back(const value_type& val)
{
  if (m_size == m_capacity)
    grow();

    //placement construct
  new (&(*this)[m_size++]) value_type(val);
}
template <class T>
void vector<T>::push_back(value_type&& val)
{
  if (m_size == m_capacity)
    grow();

    //move construct - not sure if correct?
  (*this)[m_size++] = std::move(val);
}
template <class T>
void vector<T>::pop_back()
{
  assert(m_size);
  //explicitly call destructor
  (*this)[--m_size]->~T();
}

template <class T>
vector<T>::vector(const vector&)
{ assert(false); }
template <class T>
vector<T>& vector<T>::operator=(const vector<T>&)
{
  assert(false);
  return *this;
}

template <class T>
vector<T>::vector(vector&&)
{
  assert(false);
}
template <class T>
vector<T>& vector<T>::operator=(vector&&)
{
  assert(false);
  return *this;
}

}
}
