#ifndef STACK_H
#define STACK_H

#include "StackNode.h"

template <class T>
class Stack
{
private:
    StackNode<T> *top;
    int count;

public:
    Stack();

    ~Stack();

    bool push(T&);
    bool pop(T&);
    bool getTop(T&);
    int getCount() { return count; }
    bool isEmpty() { return (!count); }
};

template <class T>
Stack<T>::Stack()
{
    top = nullptr;
    count = 0;
}

template <class T>
bool Stack<T>::push(T &newData)
{
    StackNode<T> *newNode = new StackNode<T>(newData);

    if (!newNode)
        return false;

    newNode->setNext(top);
    top = newNode;

    count++;

    return true;
}

template <class T>
bool Stack<T>::pop(T &data)
{
    StackNode<T> *temp = top;

    if (!count)
        return false;

    data = top->getData();
    top = top->getNext();
    delete temp;
    count--;
}

template <class T>
bool Stack<T>::getTop(T &data)
{
    if (!count)
        return false;

    data = top->getData();

    return true;
}

template <class T>
Stack<T>::~Stack()
{
    StackNode<T> *prev, *curr = top;

    while (curr)
    {
        prev = curr;
        curr = curr->getNext();
        delete prev;
    }
}

#endif // STACK_H