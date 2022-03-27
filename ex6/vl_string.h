//
// Created by yair.shtern on 24/06/2021.
//

#ifndef _VL_STRING_H_
#define _VL_STRING_H_
#include "vl_vector.h"

/**
 * A generic class vl_string - vector of chars.
 * @tparam StaticCapacity 16
 */
template<size_t StaticCapacity = DEFAULT_CAPACITY>
class vl_string : public vl_vector<char, StaticCapacity> {
 public:
  /**
   * Default constructor. initialize '\0'.
   */
  vl_string () : vl_vector<char, StaticCapacity> (1, '\0')
  {}
  /**
   * Copy constructor.
   */
  vl_string (const vl_string &other) :
      vl_vector<char, StaticCapacity> (other.begin (), other.end ())
  {}
  /**
   * Implicit constructor.
   */
  vl_string (const char *str) :
      vl_vector<char, StaticCapacity> (str, str + (strlen (str) + 1))
  {}
  /**
   * @return the size of the chars in the vector.
   */
  size_t size () const override
  { return this->_size - 1; }
  /**
   * Pops the last char (not \0) from the vector.
   */
  void pop_back () override
  {
    this->erase (this->end () - TWO);
  }
  /**
   * Checks if given sequence of chars is in the vector.
   * @param c a string.
   * @return true if found else return false.
   */
  bool contains (const char *c) const
  {
    return (strstr (this->data (), c) != nullptr);
  }
  /**
   * += operator.
   * @param rhs other vl_string to add.
   * @return by reference.
   */
  vl_string &operator+= (const vl_string &rhs)
  {
    this->insert (this->end () - 1, rhs.begin (), rhs.end () - 1);
    return *this;
  }
  /**
   * += operator.
   * @param str a string to add.
   * @return by reference.
   */
  vl_string &operator+= (const char *str)
  {
    this->insert (this->end () - 1, str, str + strlen (str));
    return *this;
  }
  /**
   * += operator.
   * @param c a single char to add.
   * @return by reference.
   */
  vl_string &operator+= (const char c)
  {
    this->insert (this->end () - 1, c);
    return *this;
  }
  /**
   * + operator.
   * @param rhs other vl_string to add.
   * @return by value.
   */
  vl_string operator+ (const vl_string &rhs) const
  {
    vl_string<StaticCapacity> v (*this);
    v += rhs;
    return v;
  }
  /**
   * + operator.
   * @param c a string to add.
   * @return by value.
   */
  vl_string operator+ (const char *c) const
  {
    vl_string<StaticCapacity> v (*this);
    v += c;
    return v;
  }
  /**
   * + operator.
   * @param c a single char to add.
   * @return by value.
   */
  vl_string operator+ (const char c) const
  {
    vl_string<StaticCapacity> v (*this);
    v += c;
    return v;
  }
  /**
   * Implicit casting operator.
   * @return char *.
   */
  operator const char * () const
  {
    return this->data ();
  }
};
#endif //_VL_STRING_H_
