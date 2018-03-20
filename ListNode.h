#ifndef UNTITLED_FOLDER_3_LISTNODE_H
#define UNTITLED_FOLDER_3_LISTNODE_H

template <class T>
class ListNode
{
private:
    ListNode *next;
    T data;

public:
    ListNode() { data = T(), next = nullptr; }
    ListNode(const T &data) { this->data = data; next = nullptr; };

    ListNode *getNext() { return next; }
    T getData() { return data; }

    void setNext(ListNode *next) { this->next = next; }
    void setData(T data) { this->data = data; }
};

#endif //UNTITLED_FOLDER_3_LISTNODE_H
