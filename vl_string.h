//
// Created by Matanel on 16/06/2021.
//

#ifndef _VL_STRING_H_
#define _VL_STRING_H_

#include <cstring>
#include "vl_vector.h"

template<size_t StaticCapacity = STATIC_CAP>
class vl_string : public vl_vector<char, StaticCapacity> {
 public:

  vl_string () : vl_vector<char, StaticCapacity> ()
  {
    add_null = true;
//    this->_size++;
//    this->at (size ()) = '\0';
    this->insert (this->end (), '\0');
    add_null = false;

  }

  vl_string (const vl_string &other) : vl_vector<char, StaticCapacity> (other)
  {
    add_null = false;
  }

  vl_string (const char *str) :
      vl_vector<char, StaticCapacity> (str, str + strlen (str))
  {
    add_null = true;
    this->insert (this->end (), '\0');
    add_null = false;
  }

  size_t size () const override
  {
    if (add_null)
      {
        return this->_size;
      }
    return this->_size -1;
  }

  bool empty () const override
  {
    return this->_size == 1;
  }

  bool contains (const char str) const override
  {
//    for (auto const& c : *this)
//    {
//      if (c == str[0])
//        {
//          for(size_t i = 1; i < strlen (str) ; i++)
//            {
//              if ((c+i) != str[i])
//                return false;
//            }
//        }
//    }
    return true;
  }

  vl_string &operator+= (const vl_string &other)
  {
    this->insert (this->end (), other.begin (), other.end ());
    return *this;
  }

  vl_string &operator+= (const char *str)
  {
    this->insert (this->end (), str, str + strlen (str));
    return *this;

  }

  vl_string &operator+= (const char c)
  {
    this->push_back (c);
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

 private:
  bool add_null;

};

#endif //_VL_STRING_H_
