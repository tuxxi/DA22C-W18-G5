#ifndef TEAM_PROJECT_LINKEDLIST
#define TEAM_PROJECT_LINKEDLIST

template <class T, class K>
class LinkedList {
private:
    int (*cmp)(const T*, const K&);

    struct ListNode {
        T *data;
        ListNode *next;
    };

    ListNode *head;

public:
    LinkedList(int (*cmp)(const T*, const K&));

    ~LinkedList();

    bool insert(const T*);
    bool remove(const K&);
    T *search(const K&);
};

template <class T, class K>
LinkedList<T, K>::LinkedList(int (*cmp)(const T*, const K&))
{
    head = new ListNode {0};
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
    ListNode *curr, *prev, *newNode;

    while (curr && cmp(curr, newRecord) < 0)
    {
        prev = curr;
        curr = curr->next;
    }

    if (cmp(curr, newRecord))
    {
        newNode = new ListNode;
        newNode->data = newRecord;
        prev->next = newNode;
        newNode->next = curr;
    }
}

template <class T, class K>
bool LinkedList<T, K>::remove(const K &removalKey)
{
    ListNode *curr, *prev;

    while (curr && cmp(curr, removalKey) > 0)
    {
        prev = curr;
        curr = curr->next;
    }

    if (!cmp(curr, removalKey))
    {
        prev->next = curr->next;
        delete curr;

        return true;
    }

    return false;
}

template <class T, class K>
T *LinkedList<T, K>::search(const K &searchKey)
{
    ListNode *nodePtr = head;

    while (nodePtr && cmp(nodePtr, searchKey) > 0)
        nodePtr = nodePtr->next;

    if (!cmp(nodePtr, searchKey))
        return nodePtr;

    return nullptr;
}


#endif //TEAM_PROJECT_LINKEDLIST_H
