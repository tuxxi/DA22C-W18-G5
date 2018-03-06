/** Binary Search Tree ADT
 * @author Aidan Sojourner, Feb 2018
 * @file BinarySearchTree.h
*/

#pragma once

#include <iostream>

template<class T>
class BinarySearchTree
{
public:
    // "admin" functions
    BinarySearchTree();
    virtual ~BinarySearchTree();

    //copy constructor
    BinarySearchTree(const BinarySearchTree<T>& tree);

    //copy assignment operator
    BinarySearchTree& operator= (const BinarySearchTree& sourceTree);
    //operator<< that outputs an indented list using recursive print function
    template<class C>
    friend std::ostream& operator<<(std::ostream& stream, const BinarySearchTree<C>& other);


    bool empty() const { return count == 0; }
    int size() const { return count; }
    void clear() { destroyTree(rootPtr); rootPtr = nullptr; count = 0; }

    void preOrder(void visit(T &)) const {_preorder(visit, rootPtr);}
    void inOrder(void visit(T &)) const  {_inorder(visit, rootPtr);}
    void postOrder(void visit(T &)) const{_postorder(visit, rootPtr);}

    // insert a node at the correct location
    bool insert(const T& newEntry);
    // remove a node if found
    bool remove(const T& anEntry);
    // find a target node
    T* getEntry(const T& target) const;
    // find the smallest node
    T& findSmallest() const;
    // find the largest node
    T& findLargest() const;

private:
    template<class S>
    struct BinaryNode
    {
        S item;                 // Data portion
        BinaryNode* leftPtr;	// Pointer to left child
        BinaryNode* rightPtr;// Pointer to right child

        explicit BinaryNode(const S& anItem)
            : item(anItem), leftPtr(nullptr), rightPtr(nullptr) {}
        BinaryNode(const S& anItem, BinaryNode* left, BinaryNode* right)
            : item(anItem), leftPtr(left), rightPtr(right) {}

        bool isLeaf() const {return (leftPtr == nullptr && rightPtr == nullptr);}
    };
    typedef BinaryNode<T> node;
    unsigned int count;
    node* rootPtr;

    //internal utility functions
    // delete target node from tree, called by internal remove node
    node* deleteNode(node *targetNodePtr);
    // remove the leftmost node in the left subtree of nodePtr. Used when the root of the tree is removed
    node* removeLeftmost(node *nodePtr, T &successor);
    // delete all nodes from the tree
    void destroyTree(node* nodePtr);
    // copy from the tree rooted at nodePtr and returns a pointer to the copy
    node* copyTree(const node* nodePtr);

    //internal recursive functions
    //internal insert node: insert newNode in nodePtr subtree
    node* _insert(node* nodePtr, node* newNode);
    //internal remove node: locate and delete target node under nodePtr subtree
    node* _remove(node* nodePtr, const T& target, bool& success);

    node* _find(node *treePtr, const T &target) const;
    T& _smallest(node *nodePtr) const;
    T& _largest(node* nodePtr) const;

    // internal traverse for copy
    node* _copy(const node* nodePtr);
    // internal traverse
    void _preorder(void visit(T &), node* nodePtr) const;
    void _inorder(void visit(T &), node* nodePtr) const;
    void _postorder(void visit(T &), node* nodePtr) const;

    //indented list traverse
    void _print(node* nodePtr, std::ostream& stream, int indentLevel) const;
};
/************************************************************************/
///////////////////////// public (exported) functions /////////////////////
/************************************************************************/

//default ctor
template <class T>
BinarySearchTree<T>::BinarySearchTree()
    : rootPtr(nullptr), count(0)
{}

//copy assignment ctor
template <class T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree<T>& tree)
    : rootPtr(nullptr), count(0)
{
    copyTree(tree.rootPtr);
}

//dtor
template <class T>
BinarySearchTree<T>::~BinarySearchTree()
{
    clear();
}

//tries to insert a new node into the tree
//returns true when successful
template<class T>
bool BinarySearchTree<T>::insert(const T & newEntry)
{
    this->rootPtr = _insert(this->rootPtr, new node(newEntry));
    return true;
}

//searches for a target node sorted by T, and removes it if found.
//returns true when successfully removing node, false when node was not found
template<class T>
bool BinarySearchTree<T>::remove(const T& target)
{
    bool isSuccessful = false;
    this->rootPtr = _remove(this->rootPtr, target, isSuccessful);
    return isSuccessful;
}

//searches for an entry and assigns the returnedItem reference to the item if found
//returns true when the item was found, false if not
template<class T>
T* BinarySearchTree<T>::getEntry(const T& anEntry) const
{
    auto* ptr = this->rootPtr;
    ptr = _find(ptr, anEntry);
    if (ptr && ptr->item == anEntry)
    {
        return ptr->item;
    }
    return nullptr;
}
//traverses the tree to find the smallest node in avg O(log n) time
template<class T>
T& BinarySearchTree<T>::findSmallest() const
{
    return _smallest(this->rootPtr);
}
//traverses the tree to find the largest node in avg O(log n) time
template<class T>
T& BinarySearchTree<T>::findLargest() const
{
    return _largest(this->rootPtr);
}
//operator<< for ostream
template<class T>
std::ostream &operator<<(std::ostream &stream, const BinarySearchTree<T> &other)
{
    int i = 0; //levels start at 0 :^)
    other._print(other.rootPtr, stream, i);
    return stream;
}

//copy assignment operator
template<class T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(const BinarySearchTree<T>& sourceTree)
{
    copyTree(sourceTree.rootPtr);
    return *this;
}

/************************************************************************/
//////////////////////////// private (internal) functions ///////////////////////////
/************************************************************************/

//safely removes a node from the tree
template<class T>
typename BinarySearchTree<T>::node* BinarySearchTree<T>::deleteNode(node *nodePtr)
{
    if (nodePtr->isLeaf()) //are we a leaf?
    {
        delete nodePtr;
        return nullptr;
    }
    if (nodePtr->leftPtr == nullptr) //does our node have NO left children?
    {
        node* nodeToConnectPtr = nodePtr->rightPtr;
        delete nodePtr;
        return nodeToConnectPtr;
    }
    if (nodePtr->rightPtr == nullptr) //what about right children?
    {
        node* nodeToConnectPtr = nodePtr->leftPtr;
        delete nodePtr;
        return nodeToConnectPtr;
    }
    //all above are false, we're a node with TWO children
    T newNodeValue;
    nodePtr->rightPtr = removeLeftmost(nodePtr->rightPtr, newNodeValue);
    nodePtr->item = newNodeValue;
    return nodePtr;
}

//in the special case that we are a node with two children, we need to remove the leftmost node first
template<class T>
typename BinarySearchTree<T>::node* BinarySearchTree<T>::removeLeftmost(node *nodePtr,
                                                                        T &successor)
{
    if (nodePtr->leftPtr == nullptr)
    {
        successor = nodePtr->item;
        return deleteNode(nodePtr);
    }
    nodePtr->leftPtr = removeLeftmost(nodePtr->leftPtr, successor);
    return nodePtr;

}
//internal copy that ensures we properly destroy *this before we copy a new tree
template<class T>
typename BinarySearchTree<T>::node* BinarySearchTree<T>::copyTree(const node* nodePtr)
{
    clear(); //we should clear out the original tree before we copy the new one
    rootPtr = _copy(nodePtr);
    return rootPtr;
}


//internal recursive destruction function
template<class T>
void BinarySearchTree<T>::destroyTree(node* nodePtr)
{
    if (nodePtr)
    {
        destroyTree(nodePtr->leftPtr);
        destroyTree(nodePtr->rightPtr);
        --count;
        delete nodePtr;
    }
}
//internal recursive insertion function
template<class T>
typename BinarySearchTree<T>::node* BinarySearchTree<T>::_insert(node* pParent, node* pNewChild)
{
    if (!pParent) //we cannot traverse due to null leaf ptr, root is the new node
    {
        return pNewChild;
    }
    if (pParent->item> pNewChild->item)
    {
        pParent->leftPtr = _insert(pParent->leftPtr, pNewChild);
    }
    if (pParent->item < pNewChild->item)
    {
        pParent->rightPtr = _insert(pParent->rightPtr, pNewChild);
    }
    ++this->count;
    return pParent;
}

//internal recursive node removal function
template<class T>
typename BinarySearchTree<T>::node* BinarySearchTree<T>::_remove(node* nodePtr, const T& target, bool& success)

{
    if (!nodePtr) //we reached the end of the tree without finding the target!
    {
        success = false;
        return nullptr;
    }
    if (nodePtr->item > target)
    {
        nodePtr->leftPtr = _remove(nodePtr->leftPtr, target, success);
    }
    else if (nodePtr->item < target)
    {
        nodePtr->rightPtr = _remove(nodePtr->rightPtr, target, success);
    }
    else //found target!
    {
        nodePtr = deleteNode(nodePtr);
        success = true;
    }
    --this->count;
    return nodePtr;
}
//recursive - tries to find a node with value equal to target in the tree rooted at nodePtr.
//returns pointer to the node if found, nullptr if not
template<class T>
typename BinarySearchTree<T>::node* BinarySearchTree<T>::_find(node *nodePtr, const T& target) const
{
    //base case: null check before we try to access items
    if (nodePtr == nullptr)
    {
        return nullptr;
    }
    if (nodePtr->item > target)
    {
        _find(nodePtr->leftPtr, target);
    }
    else if (nodePtr->item < target)
    {
        _find(nodePtr->rightPtr, target);
    }
    else //item == target
    {
        return nodePtr;
    }
}

//recursive traversal of right side only, the right side contains the largest node
template<class T>
T& BinarySearchTree<T>::_largest(node *nodePtr) const
{
    //the largest node is at the end of the right side
    //traverse right until the right child node is null, in which case we've reached the end
    if (nodePtr->rightPtr == nullptr)
    {
        return nodePtr->item;
    }
    return _largest(nodePtr->rightPtr);
}

//recursive traversal of left side only, the left side contains the smallest node
template<class T>
T& BinarySearchTree<T>::_smallest(node *nodePtr) const
{
    //the smallest node is at the end of the left side
    //traverse right until the right child node is null, in which case we've reached the end
    if (nodePtr->leftPtr == nullptr)
    {
        return nodePtr->item;
    }
    return _smallest(nodePtr->rightPtr);
}

//internal recursive copy function
template<class T>
typename BinarySearchTree<T>::node *BinarySearchTree<T>::_copy(const node* nodePtr)
{
    node* newNode = nullptr;
    if (nodePtr)
    {
        ++count;
        newNode = new node(nodePtr->item);
        newNode->leftPtr = _copy(nodePtr->leftPtr);
        newNode->rightPtr = _copy(nodePtr->rightPtr);
    }
    return newNode;
}
//internal preorderder traverse
template<class T>
void BinarySearchTree<T>::_preorder(void visit(T &), node* nodePtr) const
{
    if (nodePtr)
    {
        T item = nodePtr->item;
        visit(item);
        _preorder(visit, nodePtr->leftPtr);
        _preorder(visit, nodePtr->rightPtr);
    }
}

//internal inorder traverse
template<class T>
void BinarySearchTree<T>::_inorder(void visit(T &), node* nodePtr) const
{
    if (nodePtr)
    {
        _inorder(visit, nodePtr->leftPtr);
        T item = nodePtr->item;
        visit(item);
        _inorder(visit, nodePtr->rightPtr);
    }
}

//internal postorder traverse
template<class T>
void BinarySearchTree<T>::_postorder(void visit(T &), node* nodePtr) const
{
    if (nodePtr)
    {
        _postorder(visit, nodePtr->leftPtr);
        _postorder(visit, nodePtr->rightPtr);
        T item = nodePtr->item;
        visit(item);
    }
}
//internal recursive print function that outputs to std::ostream
template<class T>
void BinarySearchTree<T>::_print(node *nodePtr, std::ostream &stream, int indentLevel) const
{
    if (nodePtr)
    {
        //std::string(int n, char* c) prints "c" n times! very useful :)
        stream << std::string(indentLevel, '\t') << indentLevel++ << ". " << nodePtr->item << std::endl;

        _print(nodePtr->leftPtr, stream, indentLevel);
        _print(nodePtr->rightPtr, stream, indentLevel);
    }
}