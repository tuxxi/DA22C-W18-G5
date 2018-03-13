#ifndef TEAM_PROJECT_LINKEDLIST
#define TEAM_PROJECT_LINKEDLIST

template <class T, class K>
class LinkedList {
private:
    int (*cmp)(const T*, const K&);

    struct ListNode {
        T const *data;
        ListNode *next;
    };

    ListNode *head;

public:
    LinkedList(int (*cmp)(const T*, const K&));
    ~LinkedList();
    bool insert(const T*);
    bool remove(const K&, T& dataOut);
    bool search(const K&, T&);
};

template <class T, class K>
LinkedList<T, K>::LinkedList(int (*cmp)(const T*, const K&))
{
    head = new ListNode;
    head = nullptr;
    this->cmp = cmp;
}

template <class T, class K>
LinkedList<T, K>::~LinkedList()
{
    ListNode *curr = head, *prev;

    while (curr)
    {
        prev = curr;
        curr = curr->next;
        delete prev;
    }
}

template <class T, class K>
bool LinkedList<T, K>::insert(const T *newRecord)
{
    ListNode *newNode;
    newNode = new ListNode;
    newNode->data = newRecord;
    newNode->next = nullptr;

    if(head == nullptr)
        head = newNode;

    else
    {
        ListNode *curr = head;
        while (curr->next)
            curr = curr->next;

        curr->next = newNode;
    }

    return true;
}



template <class T, class K>
bool LinkedList<T, K>::remove(const K &removalKey, T& dataOut)
{
    ListNode *curr, *prev;

    if(head == nullptr)
        return false;

    curr = head;
    if(cmp(curr->data, removalKey) == 1)
    {
        dataOut = *curr->data;
        head = curr->next;
        delete curr;

        return true;
    }

    prev = head;
    curr = head->next;

    while (curr && cmp(curr->data, removalKey) == 0)
    {
        prev = curr;
        curr = curr->next;
    }

    if (curr && cmp(curr->data, removalKey) == 1)
    {
        dataOut = *curr->data;
        prev->next = curr->next;
        curr->data = nullptr;
        delete curr;

        return true;
    }

    return false;
}

template <class T, class K>
bool LinkedList<T, K>::search(const K &searchKey, T& dataOut)
{
    ListNode *nodePtr = head;

    while (nodePtr)
    {
        if (cmp(nodePtr->data, searchKey) == 1)
        {
            dataOut = *nodePtr->data;
            return true;
        }
        nodePtr = nodePtr->next;
    }

    return false;
}


#endif //TEAM_PROJECT_LINKEDLIST_H