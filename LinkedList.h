#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include "ListNode.h"

template <class T>
class LinkedList
{
private:
    int count;
    ListNode<T> *head;
    int (*cmp)(const T&, const T&);

public:
    LinkedList(int (*)(const T&, const T&));

    ~LinkedList();

    int getCount() { return count; }
    void displayList();
    bool insert(const T&);
    bool remove(const T&);
    bool search(T&);
};

template <class T>
LinkedList<T>::LinkedList(int (*cmp)(const T&, const T&))
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

    while (curr && cmp(curr->getData(), newData) > 0)
    {
        prev = curr;
        curr = curr->getNext();
    }

    if (curr && !cmp(curr->getData(), newData))
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

    while (curr && cmp(curr->getData(), removalKey) > 0)
    {
        prev = curr;
        curr = curr->getNext();
    }

    if (!curr || cmp(curr->getData(), removalKey))
        return false;

    prev->setNext(curr->getNext());
    delete curr;
    count--;

    return true;
}

template <class T>
bool LinkedList<T>::search(T &searchKey)
{
    ListNode<T> *curr = head->getNext();

    while (curr && cmp(curr->getData(), searchKey) > 0)
        curr = curr->getNext();

    if (!curr || cmp(curr->getData(), searchKey))
        return false;
    std::cout << curr->getData() << std::endl;
    searchKey = curr->getData();

    return true;
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