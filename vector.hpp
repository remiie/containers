#ifndef VECTOR_H
#include <iostream>
#include <memory>
#include <stdexcept>
#include "iterator.hpp"
#include "enable_if.hpp"
#include "is_integral.hpp"
/*#include <vector>
std::vector<int> i;*/
namespace ft {
    template <typename T, class Allocator = std::allocator<T> >
    class vector;
}
template <typename T, class Allocator>

class ft::vector {
public:
    typedef T                                           value_type;
    typedef size_t                                      size_type;
    typedef Allocator                                   allocator_type;
    typedef T&                                          reference;
    typedef const T&                                    const_reference;
    typedef T*                                          pointer;
    typedef const T*                                    const_pointer;
    typedef ft::rac_iterator_v<T>				        iterator;
    typedef  ft::const_rac_iterator_v<T>                const_iterator;
    typedef ft::reverse_iterator_v<T>                   reverse_iterator;
    typedef  ft::const_reverse_iterator_v<T>            const_reverse_iterator;

private:
    pointer                                             _v;
    size_type                                           _capacity;
    allocator_type                                      _allocator;
    size_type                                           _size;
public:

  /*  explicit vector(const allocator_type&_a = allocator_type())
        :_size(0), _capacity(0), _v(NULL), _allocator(_a){};*/
    //NULL pointer initialization
    explicit vector(const allocator_type&_a = allocator_type())
            :_size(0), _capacity(0), _allocator(_a) {
        _v = _allocator.allocate(_capacity);
    }; // with 0 size;
    explicit vector(size_type _s, const value_type&_value = value_type(), const allocator_type&_a = allocator_type())
        : _size(_s), _capacity(_s), _allocator(_a) {
        _v = _allocator.allocate(_s);
        for (size_type i = 0; i < _s; ++i)
            _allocator.construct(_v + i, _value);
    }
//-----------------------------------------------
    template <typename InputIterator>
    vector(InputIterator first, InputIterator last, const allocator_type&_a = allocator_type())
        :_size(0), _capacity(0), _allocator(_a) {
        _v = _allocator.allocate(0);
        this->assign(first, last);
    }

    vector(const vector&V):_size(0),_capacity(0),_allocator(V._allocator),_v(NULL) {
        *this = V;
    }
    //ITERATORS
    iterator begin() {
        return iterator(_v);
    }
    const_iterator begin() const {
        return const_iterator(_v);
    }
    iterator end() {
        return iterator(_v + _size);
    }
    const_iterator end() const {
        return const_iterator(_v + _size);
    }
    reverse_iterator rbegin() {
        return reverse_iterator(_v + _size - 1);
    }
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(_v + _size - 1);
    }
    reverse_iterator rend() {
        return reverse_iterator(_v - 1);
    }
    const_reverse_iterator rend() const {
        return const_reverse_iterator(_v - 1);
    }
    //----------------------------------------------------
    //METHODS
    void reserve(size_type s) {
        pointer new_pointer = _allocator.allocate(s);
        _capacity = s;
        for (size_type n = 0; n < _size; n++)
            new_pointer[n] = _v[n];
        _allocator.deallocate(_v, _capacity);
        _v = new_pointer;
    }

    void resize(size_type s, value_type vt = value_type()) {
        if (s > _capacity)

        while (_size != s) {
            if (s > _size)
                push_back(vt);
            else if(s < _size)
                pop_back();
        }
    }

    //simple
    size_type capacity() {
        return _capacity;
    }
    bool empty() const {
        return _size == 0;
    }
    reference front() {
        return _v[0];
    }
    const_reference front() const {
        return _v[0];
    }
    reference back() {
        return _v[_size - 1];
    }
    const_reference back() const {
        return _v[_size - 1];
    }
    void clear() {
        for (size_t i = 0; i < _size; i++)
            _allocator.destroy(&_v[i]);
        _size = 0;
    }

    size_type size() const {
        return _size;
    }

    size_type max_size() const {
        size_type ms;
        ms = _allocator.max_size();
        return ms;
    }

    iterator insert(iterator _it, const value_type&_vt) {
        if (_size == 0)
            reserve(1);
        else if (_size + 1 > _capacity)
            reserve(_capacity * 2);
        size_type   n = _it - begin();
        for (size_type i = _size - 1; i > n; i--)
            _allocator.construct(&_v[i + 1], _v[i]);
        _allocator.construct(&_v[n], _vt);
        _size++;
        return (this->_v + n);
    }

    void insert(iterator _it, size_type n, const value_type&_vt) {
        vector t_v(_it, this->end());
        iterator t_it = t_v.begin();
        while (this->end() != _it) {
            pop_back();
        }
        for (size_t i = n; i > 0; --i) {
            push_back(_vt);
        }
        while (t_it != t_v.end()) {
            push_back(*t_it);
            ++t_it;
        }
    }
   void push_back(const value_type& vt) {
        if (_size + 1 > _capacity) {
            if (_size == 0)
                _capacity = 1;
            else
                _capacity *= 2;
            pointer _v_reallocate;
            _v_reallocate = _allocator.allocate(_capacity);
            for (size_type i = 0; i < _capacity; i++)
                _allocator.construct(&_v_reallocate[i], _v[i]);
            ~vector();
            _v = _v_reallocate;
        }
        _allocator.construct(&_v[_size++],vt);
   }

   void pop_back() {
       _allocator.destroy(&_v[--this->_size]);
   }


    void assign(size_type n, const value_type& val) {
       this->clear();
       if (n > _capacity)
           _capacity = n;
       for (size_type i = 0; i < _size; i++)
           _allocator.destroy(_v + i);
       _allocator.deallocate(_v, _size);
       _v = _allocator.allocate(_capacity);
       for (size_t i = 0; i < _size; i++)
           _allocator.construct(_v + i, val);
       _size = n;
   }

   iterator erase(iterator position) {
       pointer p_v;
       iterator it = begin();
       size_type i = 0;
       while (it != position) {
           it++;
           i++;
       }
       while (i < _size - 1) {
           p_v = _v[i + 1];
           _v = p_v;
           i++;
       }
       _allocator.destroy(&_v[_size]);
       _size--;
       return it;
   }
    iterator erase (iterator first, iterator last) {
       size_type i = 0;
       size_type j = 0;
       iterator it = first;
       while (it != last) {
           it++;
           j++;
       }
       while (i != j) {
           erase(first);
           i++;
       }
       return it;
   }

    template <typename InputIterator>
    //STL MARK: Check whether it's an integral type.  If so, it's not an iterator.
    typename ft::enable_if<!ft::is_integral<InputIterator>::value, void>::type
    assign(InputIterator first, InputIterator last, int s) {
        this->clear();
        for(; first != last; ++first)
            push_back(*first);
   }

    void swap(vector &sw) {
        pointer t_v;
        size_type t_capacity;
        size_type t_size;
        allocator_type t_allocator;

        t_v = _v;
        t_size = _size;
        t_allocator = _allocator;
        t_capacity = _capacity;

        _v = sw._v;
        _size = sw._size;
        _allocator = sw._allocator;
        _capacity = sw._capacity;
        sw._allocator = t_allocator;
        sw._v = t_v;
        sw._size = t_size;
        sw._capacity = t_capacity;
    }

   reference at(size_type index) {
       if (index >= _size)
           throw std::out_of_range("out of range");
       else
           return _v[index];
   }
   const_reference at(size_type index) const {
       if (index >= _size)
           throw std::out_of_range("out of range");
       else
           return _v[index];
   }
    allocator_type get_allocator() const {
       return _allocator;
   }
   reference operator[](size_type index) {
       return _v[index];
   };

   const_reference operator[](size_type index) const {
       return _v[index];
   }

   vector& operator=(const vector&V) {
       if (this != &V)
           assign(V.begin(), V.end());
       return *this;
   }

   ~vector() {
       for (size_type i = 0; i < _size; i++)
           _allocator.destroy(&_v[i]);
       _allocator.deallocate(_v, _capacity);
   }
};

#endif // VECTOR_H
