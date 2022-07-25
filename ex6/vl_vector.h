//
// Created by yair shtern on 17/06/2021.
//

#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_
#include <cstdio>
#include <iterator>
#include <algorithm>
#include <cstring>
#define DEFAULT_CAPACITY 16UL
#define TREE 3
#define TWO 2

/**
 * A generic class vl_vector.
 * @tparam StaticCapacity 16
 */
template<class T, size_t StaticCapacity = DEFAULT_CAPACITY>
class vl_vector {
 protected:
  size_t _size;
 private:
  /**
   * update and move from static array to dynamic.
   * (when size > capacity).
   */
  void update_after_del ()
  {
    if (_size <= StaticCapacity && _capacity > StaticCapacity)
      {
        _capacity = StaticCapacity;
        std::copy (_dynamic_array, _dynamic_array + _size, _static_array);
        delete[] _dynamic_array;
      }
  }
  size_t _capacity = StaticCapacity;
  T _static_array[StaticCapacity];
  T *_dynamic_array;
  /**
   * @return the correct capacity.
   */
  size_t get_cap () const
  { return (size_t) ((TREE * _size) / TWO); }
  /**
   * allocate new dynamic array.
   */
  void allocate_array ()
  {
    _dynamic_array = new T[_capacity];
  }
  /**
   * update and move from dynamic to static array
   * (when size <= StaticCapacity).
   * @param old_size the size of the vector before the change.
   */
  void update_after_insert (size_t old_size)
  {
    if (_size > _capacity)
      {
        T *temp = data ();
        _capacity = get_cap ();
        allocate_array ();
        std::copy (temp, temp + old_size, _dynamic_array);
        if (old_size > StaticCapacity)
          {
            delete[] temp;
          }
      }
  }
 public:
  /**
   * Default constructor.
   */
  vl_vector () : _size (0)
  {}
  /**
   * Copy constructor.
   */
  vl_vector (const vl_vector &other)
      : _size (other._size), _capacity (other.capacity ())
  {
    if (_size > StaticCapacity) allocate_array ();
    std::copy (other.begin (), other.end (), data ());
  }
  /**
   * another constructor.
   * gets range of elements with two forward iterators,
   * and makes the vector from the elements in it.
   */
  template<class ForwardIterator>
  vl_vector (ForwardIterator first, ForwardIterator last):
      _size (std::distance (first, last))
  {
    if (_size > StaticCapacity)
      {
        _capacity = get_cap ();
        allocate_array ();
      }
    std::copy (first, last, data ());
  }
  vl_vector (size_t count, const T &t) : _size (count)
  {
    if (_size > StaticCapacity)
      {
        _capacity = get_cap ();
        allocate_array ();
      }
    std::fill (begin (), end (), t);
  }
  /**
   * Destructor.
   */
  virtual ~vl_vector ()
  {
    if (_capacity > StaticCapacity)
      {
        delete[] _dynamic_array;
      }
  }

  /**
   * pointer iterator. begin, end - of the current array.
   */
  typedef T *iterator;
  typedef const T *const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
  iterator begin ()
  {
    return data ();
  }
  iterator end ()
  {
    return data () + _size;
  }
  const_iterator begin () const
  {
    return data ();
  }
  const_iterator end () const
  {
    return data () + _size;
  }
  const_iterator cbegin () const
  {
    return data ();
  }
  const_iterator cend () const
  {
    return data () + _size;
  }
  reverse_iterator rbegin ()
  {
    return reverse_iterator (end ());
  }
  reverse_iterator rend ()
  {
    return reverse_iterator (begin ());
  }
  const_reverse_iterator rbegin () const
  {
    return const_reverse_iterator (end ());
  }
  const_reverse_iterator rend () const
  {
    return const_reverse_iterator (begin ());
  }
  const_reverse_iterator crbegin () const
  {
    return const_reverse_iterator (cend ());
  }
  const_reverse_iterator crend () const
  {
    return const_reverse_iterator (cbegin ());
  }

  /**
   * @return the size of the the vector.
   */
  virtual size_t size () const
  { return _size; }
  /**
   * @return the capacity of the the vector.
   */
  size_t capacity () const
  { return _capacity; }
  /**
   * @return true if size == 0.
   */
  bool empty () const
  {
    return size () == 0;
  }
  /**
   * @return the element at the i index (if legal), [by value].
   */
  T at (unsigned int i) const
  {
    if (i < 0 || i >= _size)
      {
        throw std::out_of_range ("Error: index out of range.");
      }
    return data ()[i];
  }
  /**
   * @return the element at the i index (if legal) [by reference].
   */
  T &at (unsigned int i)
  {
    if (i < 0 || i >= _size)
      {
        throw std::out_of_range ("Error: index out of range.");
      }
    return data ()[i];
  }
  /**
   * push back given value t.
   */
  void push_back (const T &t)
  {
    insert (begin () + size (), t);
  }
  /**
   * insert element t into it (iterator) location.
   */
  iterator insert (const_iterator it, const T &t)
  {
    size_t i = std::distance (cbegin (), it);
    size_t j = std::distance (begin (), end ());
    _size++;
    update_after_insert (_size - 1);
    std::move_backward (begin () + i, begin () + j, begin () + j + 1);
    std::fill (begin () + i, begin () + i + 1, t);
    return begin () + i;
  }
  /**
   * insert range of elements [first, last) into it (iterator) location.
   */
  template<class ForwardIterator>
  iterator
  insert (const_iterator it, ForwardIterator first, ForwardIterator last)
  {
    size_t n = std::distance (first, last);
    size_t i = std::distance (cbegin (), it);
    size_t j = std::distance (begin (), end ());
    _size += n;
    update_after_insert (_size - n);
    std::move_backward (begin () + i, begin () + j, begin () + j + n);
    std::copy (first, last, begin () + i);
    return begin () + i;
  }
  /**
   * Pops the last element from the vector.
   */
  virtual void pop_back ()
  {
    if (_size == 0) return;
    _size--;
    update_after_del ();
  }
  /**
   * erase the element at the it (iterator) position.
   */
  iterator erase (const_iterator it)
  {
    size_t i = std::distance (cbegin (), it);
    std::move_backward (begin () + i + 1, end (), end () - 1);
    _size--;
    update_after_del ();
    return begin () + i;
  }
  /**
   * erase range of elements [first, last) at the it (iterator) position.
   */
  iterator erase (const_iterator first, const_iterator last)
  {
    size_t i = std::distance (cbegin (), last);
    size_t n = std::distance (first, last);
    std::move_backward (begin () + i, end (), end () - n);
    _size -= n;
    update_after_del ();
    return begin () + i - n;
  }
  /**
   * clears the vector.
   */
  void clear ()
  {
    erase (begin (), begin () + size ());
  }
  /**
   * @return pointer to the current array of the vector.
   */
  T *data ()
  {
    return _capacity <= StaticCapacity ? _static_array : _dynamic_array;
  }
  /**
   * @return const pointer to the current array of the vector.
   */
  const T *data () const
  {
    return _capacity <= StaticCapacity ? _static_array : _dynamic_array;
  }
  /**
   * Checks if given element t is in the vector.
   * @return true if found else return false.
   */
  bool contains (const T &t) const
  {
    return (std::find (begin (), end (), t)) != end ();
  }
  /**
   * = operator.
   * @param rhs other vl_vector.
   * @return by reference.
   */
  vl_vector<T, StaticCapacity> &operator= (const vl_vector &rhs)
  {
    if (this == &rhs) return *this;
    if (_capacity > StaticCapacity) delete[] _dynamic_array;
    _size = rhs._size;
    _capacity = rhs.capacity ();
    if (_size > StaticCapacity) allocate_array ();
    std::copy (rhs.begin (), rhs.end (), begin ());
    return *this;
  }
  /**
   * [] operator.
   * @return the element i in the vector (by reference).
   */
  T &operator[] (int i)
  {
    return data ()[i];
  }
  /**
   * [] operator.
   * @return the element i in the vector (by value).
   */
  T operator[] (int i) const
  {
    return data ()[i];
  }
  /**
   * == operator.
   * @return true if this == rhs (other vl_vector).
   */
  bool operator== (const vl_vector &rhs) const
  {
    if ((size () != rhs.size ())
        || (capacity () != rhs.capacity ()))
      {
        return false;
      }
    return std::equal (begin (), end (), rhs.begin ());
  }
  /**
   * != operator.
   * @return true if this != rhs (other vl_vector).
   */
  bool operator!= (const vl_vector &rhs) const
  {
    return !(this->operator== (rhs));
  }
};

#endif //_VL_VECTOR_H_
