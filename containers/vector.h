#pragma once

namespace alib
{
namespace containers
{
template <class T>
class vector
{
public:
  class iterator
  {

  };
  class const_iterator {};
  vector();
  vector(size_t num_elems);
  vector(initializer_list<T>);

  vector(const vector<T>&);
  vector<T>& operator=(const vector<T>&);

  vector(vector<T>&&);
  vector<T>& operator=(vector<T>&&);

  bool operator==(const vector<T>& rhs) const;

  T& operator[](size_t index);
  const T& operator[](size_t index) const;

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

private:
};
}
}