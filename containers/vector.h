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

        base_iterator& operator++();
        base_iterator operator++(int);
        base_iterator& operator--();
        base_iterator operator--(int);

        bool operator==(const base_iterator& rhs) const;
        bool operator!=(const base_iterator& rhs) const;
        base_iterator operator+(int32 diff) const;
        base_iterator operator-(int32 diff) const;
        int32 operator-(const base_iterator& rhs) const;
        base_iterator& operator+=(int32 n);
        base_iterator& operator-=(int32 n);
        bool operator<(const base_iterator& rhs) const;
        bool operator>(const base_iterator& rhs) const;
        bool operator<=(const base_iterator& rhs) const;
        bool operator>=(const base_iterator& rhs) const;
      protected:
        value_type* m_position = nullptr;
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

        value_type& operator*();
        value_type* operator->();
        value_type& operator[](int32 offset);
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

        const value_type& operator*() const;
        const value_type* operator->() const;
        const value_type& operator[](int32 offset) const;
      };

      vector();
      vector(size_t num_elems);
      vector(initializer_list<value_type>);

      vector(const vector&);
      vector& operator=(const vector&);

      vector(vector&&);
      vector& operator=(vector&&);

      iterator begin();
      iterator end();
      const_iterator begin() const;
      const_iterator end() const;

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

      void swap(vector& x);

      void clear() noexcept;

      template <class... Args>
      iterator emplace(const_iterator position, Args&&... args);

      template <class... Args>
      void emplace_back(Args&&... args);
      void reset();
    private:
      void grow();

      shared_ptr<value_type> m_data = nullptr;
      size_t m_size = 0;
      size_t m_capacity = 0;

      //these are powers of two because my growth strategy is in powers of two
      static const size_t max_elements = 1024; //arbitrary, set higher if necessary
      static const size_t default_size = 16; //arbitrary, set higher/lower if necessary
    };

    #include "containers\vector.inl"
  }
}
