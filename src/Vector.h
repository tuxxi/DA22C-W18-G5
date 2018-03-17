#pragma once

#include <algorithm>
#include <cstring>

template<class T>
class Vector
{
public:
    Vector();
    virtual ~Vector();

    T& operator[](size_t idx);
    T& at(size_t idx) throw(std::runtime_error);

    void add(const T& item);
    bool reserve(size_t size);
    bool remove(size_t idx);
    void clear();

    size_t size() const;

    bool empty() const;

    T* begin() { return m_array; }
    T* rbegin() { return m_array + m_count - 1; }
    T* end() { return m_array + m_count; }
    T* rend() { return m_array - 1; }
private:
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
    clear();
}

template<class T>
void Vector<T>::add(const T& item)
{
    size_t newIdx = m_count;
    if (newIdx >= m_arraySize) //check if we need to resize the array
    {
        //resize to 2x the current size
        reserve(m_arraySize * 2);
    }

    //copy the new item into array
    std::memcpy(begin() + newIdx, &item, sizeof(item));
    m_count++;
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
        size_t size = sizeof(T) * (m_count - idx - 1);
        std::memmove(begin() + idx, begin() + idx + 1,  size);
        m_count--;
        return true;

    }
    return false;
}

template<class T>
void Vector<T>::clear()
{
    delete[] m_array;
    m_array = nullptr;
    m_arraySize = 0;
    m_count = 0;
}

template<class T>
T& Vector<T>::operator[](size_t idx)
{
    return at(idx);
}

template<class T>
T& Vector<T>::at(size_t idx) throw(std::runtime_error)
{
    if (idx < m_count)
    {
        return m_array[idx];
    }
    else
    {
        throw std::runtime_error("Tried to access vector element outside of bounds");
    }
}

template<class T>
bool Vector<T>::empty() const
{
    return m_count == 0;
}

template<class T>
size_t Vector<T>::size() const
{
    return m_count;
}