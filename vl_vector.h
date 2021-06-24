//
// Created by Matanel on 16/06/2021.
//

#ifndef _VL_VECTOR_H_
#define _VL_VECTOR_H_

#include <iostream>
#include <iterator>

#define STATIC_CAP 16
#define CAP_CHANGE_FACTOR 1.5
#define ERR_MSG_OUB "Index out of range"

template<class T, size_t StaticCapacity = STATIC_CAP>
class vl_vector {

 protected:
  size_t _capacity;
  size_t _size;
  bool _array_is_static;

  T _static_array[StaticCapacity];
  T *_dynamic_array;

 public:

  /*********************
  *      Iterators     *
  **********************/
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
    return _array_is_static ? _static_array + _size : _dynamic_array + _size;
  }

  const_iterator begin () const
  {
    return data ();
  }

  const_iterator end () const
  {
    return _array_is_static ? _static_array + _size : _dynamic_array + _size;
  }

  const_iterator cbegin () const
  {
    return data ();
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
      _capacity (other._capacity), _size (other._size),
      _array_is_static (other._array_is_static)
  {
    if (!_array_is_static)
      {
        _dynamic_array = new T[_capacity];
        std::copy (other.begin (), other.end (), _dynamic_array);
      }
    else
      std::copy (other.begin (), other.end (), _static_array);
  }

  /** sequence based constructor constructor */
  template<class InputIterator>
  vl_vector (InputIterator first, InputIterator last)
  {
    _capacity = StaticCapacity;
    _size = 0;
    _array_is_static = ((size_t) std::distance (first, last)
                        <= StaticCapacity);
    if (!_array_is_static)
      _dynamic_array = new T[std::distance (first, last)];
    insert (cbegin (), first, last);
  }

  /** single-value initialized constructor */
  vl_vector (const size_t count, T value) :
      _capacity ((size_t) (CAP_CHANGE_FACTOR * (double) count)), _size (count),
      _array_is_static (false)
  {
    T *array = new T[count];
    std::fill (array, array + count, value);
    _dynamic_array = array;
    if (count <= StaticCapacity)
      {
        _capacity = StaticCapacity;
        _array_is_static = true;
        std::copy (array, array + count, _static_array);
        _size = count;
        delete[] array;
      }
  }

  /** destructor */
  virtual ~vl_vector ()
  {
    if (!_array_is_static)
      delete[] _dynamic_array;
  }

  /**********************
   *       Helpers      *
   **********************/
 private:

  /**
  * changing the capacity if needed according to the instruction
  * @param num_of_new_elems size_t - number of new elements to insert
  * @return bool - ture if no cap change
  */
  bool no_cap_change_insert (const size_t num_of_new_elems)
  {
    if (_size + num_of_new_elems <= _capacity)
      return true; //no cap change
    _capacity = CAP_CHANGE_FACTOR * (_size + num_of_new_elems);
    return false; //cap change
  }

  /**
  * changing the capacity if needed according to the instruction
   * @param num_values_to_erase
   * @return bool - true if capacity changed, false otherwise
   */
  bool cap_change_erase (const size_t num_values_to_erase)
  {
    if (_array_is_static || _size - num_values_to_erase > StaticCapacity)
      return false; //no cap change, keep data in current array
    _capacity = StaticCapacity;
    return true; //cap change to StaticCapacity, move data to static array
  }

  /**
   * adding the given element to the end of the active storage
   * @param elem - T the given new element
   * @return iterator - that points the newly inserted element at the end
   */
  iterator element_push_back (const T elem)
  {
    iterator it = end ();
    *it = elem;
    _size++;
    return it;
  }

  /**
   * inserting new element is the current active storage without change cap
   * @param c_position const_iterator - representing the wanted position
   * @param elem T - the given new element
   * @return iterator - that points the newly inserted element
   */
  iterator insert_one_no_cap_change (const T *c_position, const T elem)
  {
    size_t elems_to_move = std::distance (c_position, cend ());
    if (!elems_to_move) //only adding one element to the end of the vector
      return element_push_back (elem);
    T *temp_array = new T[elems_to_move];
    std::copy (c_position, cend (), temp_array);
    auto position = (iterator) c_position;
    *position = elem;
    std::copy (temp_array, temp_array + elems_to_move, position + 1);
    delete[] temp_array;
    _size++;
    return position;
  }

  /**
   * inserting new element is the current active storage after capacity change
   * @param c_position const_iterator - representing the wanted position
   * @param elem T - the given new element
   * @return iterator - that points the newly inserted element
   */
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

  /**
   * adding the given elements to the end of the active storage
   * @param first InputIterator - points to the first element to insert
   * @param last InputIterator - points *after* the last element to insert
   * @param num_of_new_elems size_t - number of the elements to insert
   * @return iterator - that points the newly inserted element
   */
  template<class InputIterator>
  iterator elements_push_back (InputIterator first, InputIterator last,
                               size_t num_of_new_elems)
  {
    iterator original_end = end ();
    std::copy (first, last, original_end);
    _size += num_of_new_elems;
    return original_end;
  }
/**
   * inserting the new elements by the range given at the given position
   * @param c_position const_iterator - representing the wanted position
   * @param first InputIterator - points to the first element to insert
   * @param last InputIterator - points *after* the last element to insert
   * @param num_of_new_elems size_t - number of the elements to insert
   * @return iterator - that points the newly inserted element
   */
  template<class InputIterator>
  iterator insert_elems_no_cap_change (const T *c_position,
                                       InputIterator first,
                                       InputIterator last,
                                       size_t num_of_new_elems)
  {
    size_t elems_to_move = std::distance (c_position, cend ());
    if (!elems_to_move) //adding elements to the end of the vector
      {
        return elements_push_back (first, last, num_of_new_elems);
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

  /**
   * inserting the new elements by the range given at the given position
   * @param c_position const_iterator - representing the wanted position
   * @param first InputIterator - points to the first element to insert
   * @param last InputIterator - points *after* the last element to insert
   * @param num_of_new_elems size_t - number of the elements to insert
   * @return iterator - that points the newly inserted element
   */
  template<class InputIterator>
  iterator insert_elems_cap_change (const T *c_position,
                                    InputIterator first,
                                    InputIterator last,
                                    size_t num_of_new_elems)
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

  /**********************
   *       METHODS      *
   **********************/

 public:

  /**
   * get current vector size element-wise
   * @return size_t
   */
  virtual size_t size () const
  { return _size; }

  /**
   * get current vector capacity, how many elements can the vector hold
   * @return size_t
   */
  size_t capacity () const
  { return _capacity; }

  /**
   * check if there are no elements stored in the vector
   * @return bool
   */
  bool empty () const
  { return !size (); }

  /**
   * get by value the element stored in the given position
   * @param index size_t - the given position
   * @return T
   */
  T at (const size_t index) const
  {
    if (index < 0 || index >= _size)
      {
        throw std::out_of_range (ERR_MSG_OUB);
      }
    return _array_is_static ? _static_array[index] : _dynamic_array[index];
  }

  /**
   * get by reference the element stored in the given position
   * @param index size_t - the given position
   * @return T
   */
  T &at (const size_t index)
  {
    if (index < 0 || index >= _size)
      {
        throw std::out_of_range (ERR_MSG_OUB);
      }
    return _array_is_static ? _static_array[index] : _dynamic_array[index];
  }

  /**
   * insert the given element at the end of the vector
   * @param value T - the given element
   */
  virtual void push_back (const T &elem)
  {
    insert (end (), elem);
  }

  /**
   * insert the given element at the given position
   * @param c_position const_iterator - representing the wanted position
   * @param elem T - the given new element
   * @return iterator - that points the newly inserted element
   */
  iterator insert (const_iterator c_position, const T elem)
  {
    if (no_cap_change_insert (1))
      return insert_one_no_cap_change (c_position, elem);
    return insert_one_cap_change (c_position, elem);
  }

  /**
   * insert all the element between first and last at the given position
   * @param c_position const_iterator - representing the wanted position
   * @param first InputIterator - points to the first element to insert
   * @param last InputIterator - points *after* the last element to insert
   * @return iterator - that points the newly inserted element
   */
  template<class InputIterator>
  iterator insert (const_iterator c_position,
                   InputIterator first, InputIterator last)
  {
    size_t num_of_new_elems = std::distance (first, last);
    if (no_cap_change_insert (num_of_new_elems))
      return insert_elems_no_cap_change (c_position, first,
                                         last, num_of_new_elems);
    return insert_elems_cap_change (c_position, first,
                                    last, num_of_new_elems);
  }

  /**
   * removing the last stored element in the vector
   */
  virtual void pop_back ()
  {
    erase (cend () - 1);
  }

  /**
   * erasing one element from the vector at the given position
   * @param c_position const_iterator - the given position
   * @return iterator - that points to the element after the deleted element
   */
  iterator erase (const_iterator c_position)
  {
    if (cap_change_erase (1))
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

  /**
   * erasing the elements from the given range not including the last
   * @param first InputIterator - points to the first element to erase
   * @param last InputIterator - points *after* the last element to erase
   * @return iterator - that points to the element after the deleted elements
   */
  iterator erase (const_iterator first, const_iterator last)
  {
    size_t num_values_to_erase = std::distance (first, last);
    if (cap_change_erase (num_values_to_erase))
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

  /**
   * deleting all elements from the vector
   */
  virtual void clear ()
  {
    erase (begin (), end ());
  }

  /**
   * @return the current active storage
   */
  T *data ()
  {
    return _array_is_static ? _static_array : _dynamic_array;
  }

  /**
  * @return the current active storage - const version
  */
  const T *data () const
  {
    return _array_is_static ? _static_array : _dynamic_array;
  }
  /**
   * checking to see if the given element is in the vector
   * @param elem T - the given element to search for
   * @return bool - true if found, false otherwise
   */
  bool contains (const T elem) const
  {
    return std::find (begin (), end (), elem) != end ();
  }

  /*********************
  *      OPERATORS     *
  **********************/

  /**
   * changing the vector to be exactly equal to the other given vector
   * @param other vl_vector - the other vector to match
   * @return a reference to the current vector after the changes
   */
  vl_vector &operator= (const vl_vector &other)
  {
    if (this == &other)
      {
        return *this;
      }
    _capacity = other._capacity;
    _array_is_static = other._array_is_static;
    if (_array_is_static)
      {
        std::copy (other.begin (), other.end (), _static_array);
        _size = other._size;
        return *this;
      }
    if (!empty ())
      delete[] _dynamic_array;
    _dynamic_array = new T[_capacity];
    std::copy (other.begin (), other.end (), _dynamic_array);
    _size = other._size;
    return *this;

  }

  /**
   * @return the element stored at the active storage at index i given
   */
  T &operator[] (int i)
  {
    return _array_is_static ? _static_array[i] : _dynamic_array[i];
  }

  /**
  * @return the element stored at the active storage at index i given const
  */
  T operator[] (int i) const
  {
    return _array_is_static ? _static_array[i] : _dynamic_array[i];
  }

  /**
   * checking to see if the current vector is identical to another vector given
   * @param other vl_vector - the other vector to compare with
   * @return true if identical, false otherwise
   */
  bool operator== (const vl_vector &other) const
  {
    return (_size == other._size) &&
           std::equal (other.begin (), other.end (), begin ());
  }

  /**
   * checking to see if the current vector is different to another vector given
   * @param other vl_vector - the other vector to compare with
   * @return true if different, false otherwise
   */
  bool operator!= (const vl_vector &other) const
  {
    return !(other == *this);
  }

};

#endif //_VL_VECTOR_H_
