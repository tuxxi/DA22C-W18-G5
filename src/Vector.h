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

    T& operator[](size_t idx) const;
    T& at(size_t idx) const throw();

    Vector<T>& operator=(const Vector<T> &other);

    bool add(const T& item);
    bool insertAt(const T& item, int index);
    bool reserve(size_t size);
    bool remove(size_t idx);
    void clear();

    size_t size() const;

    bool empty() const;

    T* begin() { return m_array; }
    T* rbegin() { return m_array + m_count - 1; }
    T* end() { return m_array + m_count; }
    T* rend() { return m_array - 1; }

    //I do this to suppress -Wnon-template-friend
    template<class _T>
    friend std::ostream& operator<<(std::ostream& str, const Vector<_T>& vec);

private:
    void clearData();
    static const size_t DEFAULT_SIZE = 8;
    size_t m_count;
    size_t m_arraySize;
    T* m_array;
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
std::ostream& operator<<(std::ostream& str, Vector<_T>& vec)
{
    for (_T* it = vec.begin(); it != vec.end(); ++it)
    {
        str << *it;
    }
    return str;
}
