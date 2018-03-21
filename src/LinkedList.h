#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include "Util.h"

template <class T>
class ListNode
{
private:
    ListNode * next;
    T data;

public:
    ListNode() { data = T(), next = nullptr; }
    ListNode(const T &data) { this->data = data; next = nullptr; };

    ListNode *getNext() { return next; }
    T getData() { return data; }

    void setNext(ListNode *next) { this->next = next; }
    void setData(T data) { this->data = data; }
};

template <class T>
class LinkedList
{
private:
    int count;
    ListNode<T> *head;
    typedef COMPARE_FN (*cmpFn)(const T&, const T&);
    cmpFn cmp;
public:
    LinkedList(cmpFn);

    ~LinkedList();

    int getCount() const { return count; }
    void displayList();
    bool insert(const T&);
    bool remove(const T&);
    bool search(T&);

    void insertToVector(Vector<T>& v);
};

template <class T>
LinkedList<T>::LinkedList(cmpFn cmp)
    : count(0)
{
    this->head = new ListNode<T>;
    this->cmp = cmp;
}

template <class T>
void LinkedList<T>::displayList()
{
    ListNode<T> *nodePtr = head->getNext();

    while (nodePtr)
    {
        std::cout << *nodePtr->getData() << std::endl;
        nodePtr = nodePtr->getNext();
    }
}

template <class T>
bool LinkedList<T>::insert(const T &newData)
{
    ListNode<T> *prev = head, *curr = head->getNext(), *newNode;

    while (curr && cmp(curr->getData(), newData) == COMPARE_FN::LESS_THAN)
    {
        prev = curr;
        curr = curr->getNext();
    }

    if (curr && cmp(curr->getData(), newData) == COMPARE_FN::EQUAL_TO)
        return false;

    newNode = new ListNode<T>(newData);
    prev->setNext(newNode);
    newNode->setNext(curr);
    count++;

    return true;
}

template <class T>
bool LinkedList<T>::remove(const T &removalKey)
{
    ListNode<T> *prev = head, *curr = head->getNext();

    while (curr && cmp(curr->getData(), removalKey) == COMPARE_FN::LESS_THAN)
    {
        prev = curr;
        curr = curr->getNext();
    }

    if (curr && cmp(curr->getData(), removalKey) == COMPARE_FN::EQUAL_TO)
    {
        prev->setNext(curr->getNext());
        delete curr;
        count--;

        return true;
    }

    return false;
}

template <class T>
bool LinkedList<T>::search(T &searchKey)
{
    ListNode<T> *curr = head->getNext();

    while (curr && cmp(curr->getData(), searchKey) == COMPARE_FN::LESS_THAN)
        curr = curr->getNext();

    if (!curr || cmp(curr->getData(), searchKey) != COMPARE_FN::EQUAL_TO)
        return false;

    searchKey = curr->getData();

    return true;
}

template <class T>
void LinkedList<T>::insertToVector(Vector<T>& v)
{
    ListNode<T> *nodePtr = head->getNext();

    while (nodePtr)
    {
        v.add(nodePtr->getData());
        nodePtr = nodePtr->getNext();
    }
}

template <class T>
LinkedList<T>::~LinkedList()
{
    ListNode<T> *prev, *curr = head;

    while (curr)
    {
        prev = curr;
        curr = curr->getNext();
        delete prev;
    }
}

#endif