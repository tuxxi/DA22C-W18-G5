#pragma once

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <iostream>

template<class T>
class Vector
{
public:
    Vector();
    virtual ~Vector();

    Vector<T>& operator=(const Vector<T> &other);

    /** @brief utility, same as Vector::at()
     */
    T& operator[](size_t idx) const;

    /** @brief utility std::ostream insertion operator
     */
    template<class _T>    //I do this to suppress -Wnon-template-friend
    friend std::ostream& operator<<(std::ostream& str, const Vector<_T>& vec);

    /** @brief gets a reference to the element at given index
     * @param idx the array index to access
     * @return T&, a reference to the item we want
     */
    T& at(size_t idx) const throw();

    /** @brief Adds an item to the vector
     * @param item the item to add to the vector
     * @return true if successful
     */
    bool add(const T& item);

    /** @brief inserts an item at a given index to the vector
     * @param item the item to add
     * @param index the index to insert at
     * @return true if successful
     */
    bool insertAt(const T& item, int index);

    /** @brief reserve space for the vector's data, growing its array
     * @param size the new size of the vector
     * @return false if @param size <= current size if successful
     */
    bool reserve(size_t size);

    /** @brief remove an item from the vector
     * @param idx the index of the item to remove
     * @return true if successful
     */
    bool remove(size_t idx);

    /** @brief clears out the memory of the vector and re-initilizes it
     */
    void clear();

    /** @brief get the current size of the vector, or the current number of elements
     * @return the current size
     */
    size_t size() const;

    /** @brief check if the vector is empty
     * @return true if empty, false if not
     */
    bool empty() const;

    /** @brief Iterators for utility, think std::vector::iterator but without dumb encapsulation
     */
    T* begin() const { return m_array; }
    T* rbegin() const { return m_array + m_count - 1; }
    T* end() const { return m_array + m_count; }
    T* rend() const { return m_array - 1; }

private:
    /** @brief clears out all data without initiliziing a new array
     */
    void clearData();

    static const size_t DEFAULT_SIZE = 8; //default starting size, vector will grow when expanded to beyond this size
    size_t m_count; //# of items in the array
    size_t m_arraySize; //total size of array
    T* m_array; //the vector data, heap allocated with Vector::reserve()
};

template<class T>
Vector<T>::Vector()
        : m_count(0), m_arraySize(0), m_array(nullptr)
{
    reserve(DEFAULT_SIZE);
}

template<class T>
Vector<T>::~Vector()
{
    clearData();
}

template<class T>
bool Vector<T>::add(const T& item)
{
    const size_t newIdx = m_count;
    if (newIdx >= m_arraySize) //check if we need to resize the array
    {
        //resize to 2x the current size
        if (!reserve(m_arraySize * 2))
                    return false;
    }

    //copy the new item into array using T's copy constructor
    m_array[newIdx] = T(item);    
    m_count++;

    return true;
}
template<class T>
bool Vector<T>::insertAt(const T &item, int idx)
{
    if (idx < 0) return false;
    else if (idx >= m_count) //we don't need to do anything if the requested index is >= the current count, we just
    {                       //add the new item on the end like usual
        add(item);
    }
    else
    {
        //shift the vector's elements one place up
        size_t size = sizeof(T) * (m_count - idx + 1);
        std::memmove(begin() + idx, begin() + idx - 1, size);
        m_array[idx] = T(item);
    }
    m_count++;
    return true;
}
template<class T>
bool Vector<T>::reserve(size_t size)
{
    if (size > m_arraySize)
    {
        auto tmp = new T[size]{}; //reallocate the array, zero-initializing everything for fun and profit
        std::memmove(tmp, begin(), sizeof(T) * m_arraySize); //move the current array to the new array
        if (m_array) delete[] m_array; //deallocate the old array IFF it exists

        m_arraySize = size;
        m_array = tmp;
        return true;
    }
    return false;
}

template<class T>
bool Vector<T>::remove(size_t idx)
{
    if (idx < m_count)
    {
        //shift the vector's elements one place down
        const size_t size = sizeof(T) * (m_count - idx - 1);
        std::memmove(begin() + idx, begin() + idx + 1,  size);
        m_count--;
        return true;

    }
    return false;
}

template<class T>
void Vector<T>::clear()
{
    clearData();
    reserve(DEFAULT_SIZE);
}

template<class T>
T& Vector<T>::operator[](size_t idx) const
{
    return at(idx);
}

template<class T>
T& Vector<T>::at(size_t idx) const throw()
{
    if (idx < m_count)
    {
        return m_array[idx];
    }
    //we did an oopsie
    throw std::runtime_error("Tried to access vector element outside of bounds");

}

template<class T>
bool Vector<T>::empty() const
{
    return m_count == 0;
}

template <class T>
void Vector<T>::clearData()
{
    delete[] m_array;
    m_array = nullptr;
    m_arraySize = 0;
    m_count = 0;
}

template<class T>
size_t Vector<T>::size() const
{
    return m_count;
}
template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T> &other)
{
    std::memcpy(m_array, other.m_array, other.m_arraySize * sizeof(T));
    this->m_arraySize = other.m_arraySize;
    this->m_count = other.m_count;
    return *this;
}

template<class _T>
std::ostream& operator<<(std::ostream& str, const Vector<_T>& vec)
{
    for (_T* it = vec.begin(); it != vec.end(); ++it)
    {
        str << *it;
    }
    return str;
}
