//
// Created by Matanel on 16/06/2021.
//

#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_

#define STATIC_CAP 16

template<class T, size_t StaticCapacity = STATIC_CAP>
class vl_vector {

 private:
  size_t _size;
  size_t _capacity;
  bool _array_is_static;

  T _static_array[StaticCapacity];
  T *_dynamic_array;

  int cap_change_insert_by_case (size_t num_of_new_values)
  {
    if (_size + num_of_new_values <= _capacity)
      {
        if (_array_is_static)
          return 0; //no cap change, keep data in static array
        return 1; //no cap change, keep data in dynamic array
      }
    _capacity = 1.5 * (_size + num_of_new_values);
    if (_array_is_static)
      {
        return 2; //cap change, move data to a dynamic array
      }
    return 3; //cap change, move data to a *new* dynamic array
  }

  int cap_change_erase_by_case (size_t num_values_to_erase)
  {
    if (_array_is_static)
      return 0; //no cap change, keep data in static array
    if (_size - num_values_to_erase > StaticCapacity)
      return 0; //no cap change, keep data in dynamic array
    _capacity = StaticCapacity;
    return 1; //cap change to StaticCapacity, move data to static array
  }

 public:
  /**
   * TODO - DELETE THIS:
   */
//  void print_dynamic_array (T *array)
//  {
////    std::cout << "dynamic array is active: " << !_array_is_static << std::endl;
////    std::cout << "current capacity: " << _capacity << " ,current size: " << _size
////              << std::endl;
////    std::cout << "current elements in dynamic array:" << std::endl;
//    for (size_t i = 0; i < _size; ++i)
//      {
//        std::cout << array[i] << " ";
//      }
//    std::cout << std::endl;
//  }

  /*********************
  *      Iterators     *
  **********************/
  typedef T *iterator;
  typedef const T *const_iterator;
  typedef std::reverse_iterator<iterator> reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  iterator begin ()
  {
    return _array_is_static ? _static_array : _dynamic_array;
  }

  iterator end ()
  {
    return _array_is_static ? _static_array + _size : _dynamic_array + _size;
  }

  const_iterator begin () const
  {
    return _array_is_static ? _static_array : _dynamic_array;
  }

  const_iterator end () const
  {
    return _array_is_static ? _static_array + _size : _dynamic_array + _size;
  }

  const_iterator cbegin () const
  {
    return _array_is_static ? _static_array : _dynamic_array;
  }

  const_iterator cend () const
  {
    return _array_is_static ? _static_array + _size : _dynamic_array + _size;
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
    return const_reverse_iterator (cend ());
  }

  const_reverse_iterator rend () const
  {
    return const_reverse_iterator (cbegin ());
  }

  const_reverse_iterator crbegin () const
  {
    return const_reverse_iterator (cend ());
  }

  const_reverse_iterator crend () const
  {
    return const_reverse_iterator (cbegin ());
  }

  /**********************
   *    CONSTRUCTORS    *
   **********************/

  /** default constructor */
  vl_vector () :
      _capacity (StaticCapacity), _size (0), _array_is_static (true)
  {}

  /** copy constructor */
  vl_vector (vl_vector<T, StaticCapacity> const &other) :
      _size (other._size), _capacity (other._capacity),
      _array_is_static (other._array_is_static)
  {
    for (const auto &val : other)
      {
        push_back (val);
      }
  }

  /** sequence based constructor constructor */
  template<class InputIterator>
  vl_vector (InputIterator first, InputIterator last)
  {
    _capacity = StaticCapacity;
    _size = 0;
    _array_is_static = (std::distance (first, last) <= StaticCapacity);
    if (!_array_is_static)
      _dynamic_array = new T[std::distance (first, last)];
    insert (cbegin (), first, last);
  }

  /** single-value initialized constructor */
  vl_vector (const size_t count, T value) :
      _capacity (StaticCapacity), _size (0), _array_is_static (true)
  {
//    insert
  }

  /** destructor */
  ~vl_vector ()
  {
    if (!_array_is_static)
      delete[] _dynamic_array;
  }

  /**********************
   *       METHODS      *
   **********************/
 private:
  iterator push_back_element (const T elem)
  {
    iterator it = end ();
    *it = elem;
    _size++;
    return it;
  }

  iterator insert_one_no_cap_change (const T *c_position, const T elem)
  {
    size_t elems_to_move = std::distance (c_position, cend ());
    if (!elems_to_move)
      return push_back_element (elem);
    T *temp_array = new T[elems_to_move];
    std::copy (c_position, cend (), temp_array);
    auto position = (iterator) c_position;
    *position = elem;
    std::copy (temp_array, temp_array + elems_to_move, position + 1);
    delete[] temp_array;
    _size++;
    return position;
  }

  iterator insert_one_cap_change (const T *c_position, const T elem)
  {
    T *new_array = new T[_capacity];
    iterator last_iter = std::copy (cbegin (), c_position, new_array);
    *last_iter = elem;
    std::copy (c_position, cend (), last_iter + 1);
    if (!_array_is_static)
      delete[] _dynamic_array;
    _dynamic_array = new_array;
    _size++;
    _array_is_static = false;
    return last_iter;
  }

  template<class InputIterator>
  iterator
  insert_elems_no_cap_change (const T *c_position, InputIterator first,
                              InputIterator last, size_t num_of_new_elems)
  {
    size_t elems_to_move = std::distance (c_position, cend ());
    if (!elems_to_move)
      {
        iterator original_end = end ();
        std::copy (first, last, original_end);
        _size += num_of_new_elems;
        return original_end;
      }
    T *temp_array = new T[elems_to_move];
    std::copy (c_position, cend (), temp_array);
    auto position = (iterator) c_position;
    iterator last_new_elem = std::copy (first, last, position);
    std::copy (temp_array, temp_array + elems_to_move, last_new_elem);
    delete[] temp_array;
    _size += num_of_new_elems;
    return position;
  }

  template<class InputIterator>
  iterator
  insert_elems_cap_change (const T *c_position, InputIterator first,
                           InputIterator last, size_t num_of_new_elems)
  {
    T *new_array = new T[_capacity];
    iterator last_iter = std::copy (cbegin (), c_position, new_array);
    last_iter = std::copy (first, last, last_iter);
    std::copy (c_position, cend (), last_iter);
    if (!_array_is_static)
      delete[] _dynamic_array;
    _dynamic_array = new_array;
    _size += num_of_new_elems;
    _array_is_static = false;
    return last_iter - num_of_new_elems;
  }

 public:

  size_t size () const
  { return _size; }

  size_t capacity () const
  { return _capacity; }

  bool empty () const
  { return _size == 0; }

  T at (size_t index) const
  {
    if (index < 0 || index >= _size)
      {
        throw std::out_of_range("Index out of range");
      }
    return _array_is_static ? _static_array[index] : _dynamic_array[index];
  }
  T &at (size_t index)
  {
    if (index < 0 || index >= _size)
      {
        throw std::out_of_range("Index out of range");
      }
    return _array_is_static ? _static_array[index] : _dynamic_array[index];
  }

  void push_back (const T &value)
  {
    insert (end (), value);
  }

 public:

//  todo: make sure return type.
  iterator insert (const_iterator c_position, const T elem)
  {
    switch (cap_change_insert_by_case (1))
      {
        case 0:
        case 1:
          {
            return insert_one_no_cap_change (c_position, elem);
          }
        case 2:
        case 3:
          {
            return insert_one_cap_change (c_position, elem);
          }
        default: return nullptr;
      }

  }

  template<class InputIterator>
  iterator insert (const_iterator c_position,
                   InputIterator first, InputIterator last)
  {
    size_t num_of_new_elems = std::distance (first, last);
    switch (cap_change_insert_by_case (num_of_new_elems))
      {
        case 0:
        case 1:
          {
            return insert_elems_no_cap_change (c_position, first,
                                               last, num_of_new_elems);
          }
        case 2:
        case 3:
          {
            return insert_elems_cap_change (c_position, first,
                                            last, num_of_new_elems);
          }
        default: return nullptr;
      }
  }

  void pop_back ()
  {
    erase (cend () - 1);
  }

  iterator erase (const_iterator c_position)
  {
    if (cap_change_erase_by_case (1))
      {
        iterator last_iter = std::copy (cbegin (), c_position, _static_array);
        std::copy (c_position + 1, cend (), last_iter);
        delete[] _dynamic_array;
        _array_is_static = true;
        _size -= 1;
        return last_iter;
      }
    std::copy (c_position + 1, cend (), (iterator) c_position);
    _size -= 1;
    return (iterator) c_position;
  }

  iterator erase (const_iterator first, const_iterator last)
  {
    size_t num_values_to_erase = std::distance (first, last);
    if (cap_change_erase_by_case (num_values_to_erase))
      {
        iterator last_iter = std::copy (cbegin (), first, _static_array);
        std::copy (last, cend (), last_iter);
        delete[] _dynamic_array;
        _array_is_static = true;
        _size -= num_values_to_erase;
        return last_iter;
      }
    std::copy (last, cend (), (iterator) first);
    _size -= num_values_to_erase;
    return (iterator) first;
  }

  void clear ()
  {
    erase (begin (), end ());
  }

  T *data ()
  {
    return _array_is_static ? _static_array : _dynamic_array;
  }
  const T *data () const
  {
    return _array_is_static ? _static_array : _dynamic_array;
  }
  bool contains (const T &val) const
  {
    return std::find (begin (), end (), val) != end ();
  }

  /*********************
  *      OPERATORS     *
  **********************/



  T &operator[] (int i)
  {
    return _array_is_static ? _static_array[i] : _dynamic_array[i];
  }

  T operator[] (int i) const
  {
    return _array_is_static ? _static_array[i] : _dynamic_array[i];
  }

  bool operator== (const vl_vector &other) const
  {
    return (_size == other._size) &&
           std::equal (other.begin (), other.end (), begin ());
  }
  bool operator!= (const vl_vector &rhs) const
  {
    return rhs != *this;
  }

};

#endif //_VL_VECTOR_H_
