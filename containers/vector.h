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
  
  class base_iterator
  {
  private:
    base_iterator(value_type* value) : m_position(value) {}
  public:
    base_iterator() : m_position(nullptr) {}
    ~base_iterator() = default;

    base_iterator(const base_iterator& rhs) = default;
    base_iterator& operator=(const base_iterator& rhs) = default;

    base_iterator& operator++()
    {
      ++m_position;
      return *this;
    }
    base_iterator operator++(int)
    {
      base_iterator temp(*this);
      ++m_position;
      return temp;
    }

    base_iterator& operator--()
    {
      --m_position;
      return *this;
    }
    base_iterator operator--(int)
    {
      base_iterator temp(*this);
      --m_position;
      return temp;
    }

    bool operator==(const base_iterator& rhs)
    {
      return (m_position == rhs.m_position);
    }
    bool operator!=(const base_iterator& rhs)
    {
      return (m_position != rhs.m_position);
    }
    base_iterator operator+(int32 diff)
    {
      return base_iterator(m_position + diff);
    }
    base_iterator operator-(int32 diff)
    {
      return base_iterator(m_position - diff);
    }
    int32 operator-(const base_iterator& rhs)
    {
      return m_position - rhs.m_position;
    }
    base_iterator& operator+=(int32 n)
    {
      m_position += n;
      return *this;
    }
    base_iterator& operator-=(int32 n)
    {
      m_position -= n;
      return *this;
    }
    bool operator<(const base_iterator& rhs)
    {
      return (m_position - rhs.m_position) < 0;
    }
    bool operator>(const base_iterator& rhs)
    {
      return (m_position - rhs.m_position) > 0;
    }
    bool operator<=(const base_iterator& rhs)
    {
      return (m_position - rhs.m_position) <= 0;
    }
    bool operator>=(const base_iterator& rhs)
    {
      return (m_position - rhs.m_position) >= 0;
    }
  protected:
    value_type* m_position;
  private:
    friend class vector<value_type>;
  };

  class iterator : public base_iterator
  {
  private:
    iterator(value_type* value) : base_iterator(value) {}
    friend class vector<value_type>;
  public:
    iterator() : base_iterator() {}
    ~iterator() = default;

    iterator(const iterator& rhs) = default;
    iterator& operator=(const iterator& rhs) = default;

    value_type& operator*()
    {
      return *m_position;
    }
    value_type* operator->()
    {
      return m_position;
    }
    value_type& operator[](int32 offset)
    {
      return m_position[offset];
    }
  };

  class const_iterator : public base_iterator
  {
  private:
    const_iterator(value_type* value) : base_iterator(value) {}
    friend class vector<value_type>;
  public:
    const_iterator() : base_iterator() {}
    ~const_iterator() = default;

    const_iterator(const const_iterator& rhs) = default;
    const_iterator& operator=(const const_iterator& rhs) = default;

    const value_type& operator*() const
    {
      return *m_position;
    }
    const value_type* operator->() const
    {
      return m_position;
    }
    const value_type& operator[](int32 offset) const
    {
      return m_position[offset];
    }
  };

  vector();
  vector(size_t num_elems);
  vector(initializer_list<value_type>);

  vector(const vector&);
  vector& operator=(const vector&);

  vector(vector&&);
  vector& operator=(vector&&);

  iterator begin()
  {
    return iterator(m_data.get());
  }
  iterator end()
  {
    return iterator(m_data.get() + m_size);
  }
  const_iterator begin() const
  {
    return const_iterator(m_data.get());
  }
  const_iterator end() const
  {
    return const_iterator(m_data.get() + m_size);
  }

  iterator rbegin();
  iterator rend();
  const_iterator rbegin() const;
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

  void swap(vector& x)
  {
    vector temp(*this);
    m_data = x.m_data;
    m_size = x.m_size;
    m_capacity = x.m_capacity;
    x = temp;
  }
  void clear() noexcept
  {
    while (m_size) pop_back();
  }
  
  template <class... Args>
  iterator emplace(const_iterator position, Args&&... args);
  template <class... Args>
  void emplace_back(Args&&... args)
  {
    initializer_list<T> list = { args... };
    for (auto it : list)
      push_back(it);
  }
  void reset();
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
  size_t size = sizeof(T) * num_elements;
  shared_ptr<T> ret((T*)(new char[size]));
  std::memset(ret.get(), 0, size);
  return ret;
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

}
}
