/** Binary Search Tree ADT
 * @author Aidan Sojourner, Feb 2018
 * @file BinarySearchTree.h
 */

#pragma once

#include "Vector.h"
#include "Util.h"

template<class T>
class BinarySearchTree
{
public:
    typedef COMPARE_FN(*cmpFn)(const T&, const T&); //typdef for signature of compare function
    
    explicit BinarySearchTree(cmpFn fn);
    virtual ~BinarySearchTree();
    
    //copy constructor
    BinarySearchTree(const BinarySearchTree<T>& tree);
    //copy assignment operator
    BinarySearchTree& operator= (const BinarySearchTree& sourceTree);
    //operator<< that outputs an indented list using recursive print function
    template<class C>
    friend std::ostream& operator<<(std::ostream& stream, const BinarySearchTree<C>& other);

    /** @brief checks if tree is empty
     * @return true if tree is empty, false if not
     */
    bool empty() const { return count == 0; }
    /** @brief the total size of the tree
     */
    int size() const { return count; }
    /** @brief clears out data in tree and resets it to default state
     */
    void clear() { _destroy(rootPtr); rootPtr = nullptr; count = 0; }

    /** @brief insert a new node into the tree
     * @return false if failed, true if success
     */
    bool insert(const T& newEntry);
    /** @brief tries to remove a node with search key @param anEntry
     * @return true if the node was removed
     */
    bool remove(const T& anEntry);
    /** @brief searches for all entries which match @param target
     * @param results a vector of items that returned COMPARE_FN::EQUAL_TO with the target
     * @return true if anything at all was found, false if no entries
     */
    bool findAllEntries(const T& target, Vector<T>& results) const;
    // find the smallest node
    T& findSmallest() const;
    // find the largest node
    T& findLargest() const;
    //print indended
    void printIndented(void visit(const T&, const int level)) const { _preorder(visit, rootPtr, 0); }
    /** @brief insert items inorder into a vector, allowing us to process them later
     * @param v the vector to insert items into
     */
    void insertInorder(Vector<T>& v);
private:
    struct BinaryNode
    {
        T item;                 // Data
        BinaryNode* leftPtr;	// Pointer to left child
        BinaryNode* rightPtr;   // Pointer to right child
        
        explicit BinaryNode(const T& anItem)
        : item(anItem), leftPtr(nullptr), rightPtr(nullptr) {}
        BinaryNode(const T& anItem, BinaryNode* left, BinaryNode* right)
        : item(anItem), leftPtr(left), rightPtr(right) {}
        
        bool isLeaf() const { return (leftPtr == nullptr && rightPtr == nullptr);}
    };
    typedef typename BinarySearchTree<T>::BinaryNode node;
    
    cmpFn compare;
    
    unsigned int count;
    node* rootPtr;
    
    //internal utility functions
    // delete target node from tree, called by internal remove node
    node* deleteNode(node *targetNodePtr);
    // remove the leftmost node in the left subtree of nodePtr. Used when the root of the tree is removed
    node* removeLeftmost(node *nodePtr, T &successor);
    // delete all nodes from the tree
    void _destroy(node *nodePtr);
    // copy from the tree rooted at nodePtr and returns a pointer to the copy
    node* copyTree(const node* nodePtr);
    
    //internal recursive functions
    //internal insert node: insert newNode in nodePtr subtree
    node* _insert(node* nodePtr, node* newNode);
    //internal remove node: locate and delete target node under nodePtr subtree
    node* _remove(node* nodePtr, const T& target, bool& success);

    //internal search for target node
    bool _find(node *treePtr, const T &target, Vector<T>& results) const;
    //internal recurse for smallest
    T& _smallest(node *nodePtr) const;
    //internal recurse for largest
    T& _largest(node* nodePtr) const;
    
    // internal traverse for copy
    node* _copy(const node* nodePtr);
    // internal traverse to insert into vector
    void _insert_inorder(node* nodePtr, Vector<T>& v);
    //indented list traverse
    void _print(node* nodePtr, std::ostream& stream, int indentLevel) const;
    //indented print
    void _preorder(void visit(const T &, const int level), BinaryNode *nodePtr, int floor) const;

};
/************************************************************************/
///////////////////////// public (exported) functions /////////////////////
/************************************************************************/

//default ctor
template <class T>
BinarySearchTree<T>::BinarySearchTree(cmpFn fn)
: rootPtr(nullptr), count(0), compare(fn)
{}

//copy assignment ctor
template <class T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree<T>& tree)
: rootPtr(nullptr), count(0)
{
    compare = tree.compare;
    copyTree(tree.rootPtr);
}

//dtor
template <class T>
BinarySearchTree<T>::~BinarySearchTree()
{
    clear();
}

//tries to insert a new node into the tree
//if the item's compare key is a duplicate, the item is inserted in the right subtree
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
    bool success = false;
    this->rootPtr = _remove(this->rootPtr, target, success);
    return success;
}

//searches for an entry in the BST
//returns a pointer to the item, indicating that the search may be unsuccessful
//in which case, returns a null pointer
template<class T>
bool BinarySearchTree<T>::findAllEntries(const T &anEntry, Vector<T> &results) const
{
    return _find(this->rootPtr, anEntry, results);
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

template <class T>
void BinarySearchTree<T>::insertInorder(Vector<T>& v)
{
    _insert_inorder(this->rootPtr, v);
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
    compare = sourceTree.compare;
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
typename BinarySearchTree<T>::node* BinarySearchTree<T>::removeLeftmost(node *nodePtr, T& successor)
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
void BinarySearchTree<T>::_destroy(node *nodePtr)
{
    if (nodePtr)
    {
        _destroy(nodePtr->leftPtr);
        _destroy(nodePtr->rightPtr);
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
    if (compare(pParent->item, pNewChild->item) == COMPARE_FN::GREATER_THAN)
    {
        pParent->leftPtr = _insert(pParent->leftPtr, pNewChild);
    }
    else if (compare(pParent->item, pNewChild->item) == COMPARE_FN::LESS_THAN)
    {
        pParent->rightPtr = _insert(pParent->rightPtr, pNewChild);
    }
    else //if (compare(pParent->item, pNewChild->item) == COMPARE_FN::EQUAL_TO)
    {
        //put all duplicates in the right subtree
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
    
    if (compare(nodePtr->item, target) == COMPARE_FN::GREATER_THAN)
    {
        nodePtr->leftPtr = _remove(nodePtr->leftPtr, target, success);
    }
    else if (compare(nodePtr->item, target) == COMPARE_FN::LESS_THAN)
    {
        nodePtr->rightPtr = _remove(nodePtr->rightPtr, target, success);
    }
    //check if we found the right value
    else if (compare(nodePtr->item, target) == COMPARE_FN::EQUAL_OBJECT)
    {
        nodePtr = deleteNode(nodePtr);
        success = true;
        --this->count;
    }
    else //we didn't find it, we have to keep searching in the right subtree
    {
        nodePtr->rightPtr = _remove(nodePtr->rightPtr, target, success);
    }
    
    return nodePtr;
}
//recursive - tries to find all nodes with value equal to target in the tree rooted at nodePtr.
/** @param results a vector of the items that were found for the search key
 * @return true if any nodes were found.
 */
template<class T>
bool BinarySearchTree<T>::_find(node *nodePtr, const T& target, Vector<T>& results) const
{
    //base case: null check before we try to access items
    if (!nodePtr) return false;
    
    //check if we found the correct item first
    if (compare(nodePtr->item, target) == COMPARE_FN::EQUAL_TO)
    {
        results.add(nodePtr->item);
        // search the right subtree for duplicates of the target item
        _find(nodePtr->rightPtr, target, results);
        return true;
    }
    else if (compare(nodePtr->item, target) == COMPARE_FN::GREATER_THAN)
    {
        _find(nodePtr->leftPtr, target, results);
    }
    else if (compare(nodePtr->item, target) == COMPARE_FN::LESS_THAN)
    {
        _find(nodePtr->rightPtr, target, results);
    }
    else
    {
        // search the right subtree for duplicates of the target item
        _find(nodePtr->rightPtr, target, results);
    }
    
}

//recursive traversal of right side only, the right side contains the largest node
template<class T>
T& BinarySearchTree<T>::_largest(node *nodePtr) const
{
    //the largest node is at the end of the right side
    //traverse right until the right child node is null, in which case we've reached the end
    if (!nodePtr->rightPtr)
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
    if (!nodePtr->leftPtr)
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
template <class T>
void BinarySearchTree<T>::_insert_inorder(node* nodePtr, Vector<T>& v)
{
    if (nodePtr)
    {
        _insert_inorder(nodePtr->leftPtr, v);
        v.add(nodePtr->item);
        _insert_inorder(nodePtr->rightPtr, v);
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

//this function prints data indented
template<class T>
void BinarySearchTree<T>::_preorder(void visit(const T&, const int level), BinaryNode *nodePtr, int floor) const
{
    if (nodePtr != 0)
    {
         visit(nodePtr->item, floor);
        _preorder(visit, nodePtr->leftPtr, floor++);
        _preorder(visit, nodePtr->rightPtr, floor++);
    }
}
