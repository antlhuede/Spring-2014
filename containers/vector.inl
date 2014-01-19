#pragma once

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

template <typename T>
void vector<T>::reserve(size_t n)
{
  if (m_capacity < n)
  {
    m_capacity = n;
    shared_ptr<T> temp = allocate_memory<T>(m_capacity);
    for (size_t i = 0; i < m_size; ++i)
      temp.get()[i] = std::move((*this)[i]);
    m_data = temp;
  }
}

template <typename T>
void vector<T>::swap(vector<T>& x)
{
  shared_ptr<T> temp_data(m_data);
  size_t temp_size = m_size;
  size_t temp_capacity = m_capacity;

  m_data = x.m_data;
  m_size = x.m_size;
  m_capacity = x.m_capacity;

  x.m_data = temp_data;
  x.m_size = temp_size;
  x.m_capacity = temp_capacity;
}
//insertion
template <class T>
void vector<T>::grow()
{
  m_capacity = get_next_power_of_two(m_capacity);
  assert(m_capacity <= max_elements);
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
template <class T>
void vector<T>::reset()
{
  clear();
  m_data.reset();
  m_capacity = default_size;
  m_data = allocate_memory<T>(m_capacity);
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

template <class T>
void vector<T>::push_back(const value_type& value)
{
  if (m_size == m_capacity) grow();
  new (m_data.get() + m_size++) value_type(value);
}
template <class T>
void vector<T>::push_back(value_type&& value)
{
  if (m_size == m_capacity) grow();
  (*this)[m_size++] = std::move(value);
}
template <class T>
void vector<T>::pop_back()
{
  assert(m_size);
  //explicitly call destructor
  (*this)[--m_size].~T();
}

template <class T>
vector<T>::vector(const vector<T>& rhs)
  : m_size(rhs.m_size)
  , m_capacity(rhs.m_capacity)
  , m_data(allocate_memory<T>(rhs.m_capacity))
{
  for (size_t i = 0; i < m_size; ++i)
    new (&(*this)[i]) value_type(rhs[i]);
}
template <class T>
vector<T>& vector<T>::operator=(const vector<T>& rhs)
{
  if (m_data.get() == rhs.m_data.get())
    return *this;

  clear();
  while (m_capacity <= rhs.m_capacity)
    grow();

  m_size = rhs.m_size;
  for (size_t i = 0; i < m_size; ++i)
    new (&(*this)[i]) value_type(rhs[i]);
  return *this;
}

template <class T>
vector<T>::vector(vector&& rval)
{
  m_data = rval.m_data;
  m_capacity = rval.m_capacity;
  m_size = rval.m_size;
}
template <class T>
vector<T>& vector<T>::operator=(vector&& rval)
{
  m_data = rval.m_data;
  m_capacity = rval.m_capacity;
  m_size = rval.m_size;
  return *this;
}

template <class T>
auto vector<T>::base_iterator::operator++() -> base_iterator&
{
  ++m_position;
  return *this;
}
template <class T>
auto vector<T>::base_iterator::operator++(int) -> base_iterator
{
  base_iterator temp(*this);
  ++m_position;
  return temp;
}

template <class T>
auto vector<T>::base_iterator::operator--() -> base_iterator&
{
  --m_position;
  return *this;
}
template <class T>
auto vector<T>::base_iterator::operator--(int) -> base_iterator
{
  base_iterator temp(*this);
  --m_position;
  return temp;
}

template <class T>
bool vector<T>::base_iterator::operator==(const base_iterator& rhs) const
{
  return (m_position == rhs.m_position);
}
template <class T>
bool vector<T>::base_iterator::operator!=(const base_iterator& rhs) const
{
  return (m_position != rhs.m_position);
}
template <class T>
auto vector<T>::base_iterator::operator+(int32 diff) const -> base_iterator
{
  return base_iterator(m_position + diff);
}
template <class T>
auto vector<T>::base_iterator::operator-(int32 diff) const -> base_iterator
{
  return base_iterator(m_position - diff);
}
template <class T>
int32 vector<T>::base_iterator::operator-(const base_iterator& rhs) const
{
  return m_position - rhs.m_position;
}
template <class T>
auto vector<T>::base_iterator::operator+=(int32 n) -> base_iterator&
{
  m_position += n;
  return *this;
}
template <class T>
auto vector<T>::base_iterator::operator-=(int32 n) -> base_iterator&
{
  m_position -= n;
  return *this;
}
template <class T>
bool vector<T>::base_iterator::operator<(const base_iterator& rhs) const
{
  return (m_position - rhs.m_position) < 0;
}
template <class T>
bool vector<T>::base_iterator::operator>(const base_iterator& rhs) const
{
  return (m_position - rhs.m_position) > 0;
}
template <class T>
bool vector<T>::base_iterator::operator<=(const base_iterator& rhs) const
{
  return (m_position - rhs.m_position) <= 0;
}
template <class T>
bool vector<T>::base_iterator::operator>=(const base_iterator& rhs) const
{
  return (m_position - rhs.m_position) >= 0;
}


template <class T>
auto vector<T>::iterator::operator*() -> value_type&
{
  return *m_position;
}
template <class T>
auto vector<T>::iterator::operator->() -> value_type*
{
  return m_position;
}
template <class T>
auto vector<T>::iterator::operator[](int32 offset) -> value_type&
{
  return m_position[offset];
}

template <class T>
auto vector<T>::iterator::operator*() const -> const value_type&
{
  return *m_position;
}
template <class T>
auto vector<T>::iterator::operator->() const -> const value_type*
{
  return m_position;
}
template <class T>
auto vector<T>::iterator::operator[](int32 offset) const -> const value_type&
{
  return m_position[offset];
}
