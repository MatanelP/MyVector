//
// Created by Matanel on 16/06/2021.
//
// SPECIALIZATION OF VL_VECTOR FOR STRINGS OF CHAR
//
// please see vl_vector.h for references

#ifndef _VL_STRING_H_
#define _VL_STRING_H_

#include <cstring>
#include "vl_vector.h"

template<size_t StaticCapacity = STATIC_CAP>
class vl_string : public vl_vector<char, StaticCapacity> {
 public:

  vl_string () : vl_vector<char, StaticCapacity> ()
  {
    this->data ()[0] = '\0';
    this->_size++;
  }

  vl_string (const vl_string &other) : vl_vector<char, StaticCapacity> (other)
  {}

  vl_string (const char *str) :
      vl_vector<char, StaticCapacity> (str, str + strlen (str) + 1)
  {
    this->data ()[size ()] = '\0';
  }

  size_t size () const override
  {
    return this->_size - 1;
  }

  void push_back (const char &value) override
  {
    this->insert (this->end () - 1, value);
  }

  void pop_back () override
  {
    this->erase (this->cend () - 2);
  }

  void clear () override
  {
    this->erase (this->begin (), this->end () - 1);
  }

  bool contains (const char *str) const
  {
    for (size_t i = 0; i < this->_size; ++i)
      {
        if (this->at (i) == str[0])
          {
            for (size_t j = 1; j < strlen (str); ++j)
              {
                if (this->at (i + j) != str[j])
                  return false;
              }
            return true;
          }
      }
    return false;

  }

  vl_string &operator+= (const vl_string &other)
  {
    this->insert (this->end () - 1, other.begin (), other.end () - 1);
    return *this;
  }

  vl_string &operator+= (const char *str)
  {
    this->insert (this->end () - 1, str, str + strlen (str));
    return *this;
  }

  vl_string &operator+= (const char c)
  {
    push_back (c);
    return *this;
  }

  vl_string operator+ (const vl_string &other)
  {
    return vl_string<> (*this += other);
  }

  vl_string operator+ (const char *str)
  {
    return vl_string<> (*this += str);

  }

  vl_string operator+ (const char c)
  {
    return vl_string<> (*this += c);
  }

  operator const char * () const
  {
    return this->data ();
  }

};

#endif //_VL_STRING_H_
