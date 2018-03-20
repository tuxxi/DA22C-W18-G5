#ifndef STACKNODE_H
#define STACKNODE_H

template <class T>
class StackNode
{
private:
    StackNode *next;
    T data;

public:
    StackNode() { data = T(); next = nullptr; }
    StackNode(const T &data) { this->data = data; next = nullptr; }

    StackNode *getNext() { return next; }
    T getData() { return data; }

    void setNext(StackNode *next) { this->next = next; }
    void setData(T data) { this->data = data; }
};

#endif //STACKNODE_H
