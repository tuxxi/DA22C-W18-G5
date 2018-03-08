#pragma once
#ifdef _BST_TEST //don't compile this code if we're not testing stuff
//you can define this parameter on the command line with '-D_BST_TEST'
//or in cmake with 'add_definitions(-D_BST_TEST)'

// main test driver for BST
// Created by Frank M. Carrano and Tim Henry.
// modified by Aidan Sojourner

#include "../BinarySearchTree.h"
#include <iostream>
#include <string>

COMPARE_FN compare(const std::string& a, const std::string& b)
{
    if (a > b) return COMPARE_FN::GREATER_THAN;
    else if (a < b) return COMPARE_FN::LESS_THAN;
    else return COMPARE_FN::EQUAL_TO;
}

// display function to pass to BST traverse functions
void display(std::string& anItem)
{
    std::cout << "Displaying item - " << anItem << std::endl;
}
void check(bool success)
{
    if (success)
        std::cout << "Done." << std::endl;
    else
        std::cout << " Entry not in tree." << std::endl;
}
void doBSTTests()
{

    BinarySearchTree<std::string> tree1(compare);
    tree1.insert("10");		// 10
    tree1.insert("20");		//	 |
    tree1.insert("30");		//    20
    tree1.insert("40");		//		|
    tree1.insert("50");		//		 30
    //		   |
    //          40
    //			  |
    //			   50

    std::cout << "Tree 1 Preorder: Should be 10 20 30 40 50\n";
    tree1.preOrder(display);
    std::cout << "Tree 1 Inorder: Should be 10 20 30 40 50\n";
    tree1.inOrder(display);
    std::cout << "Tree 1 Postorder: Should be 50 40 30 20 10\n";
    tree1.postOrder(display);
    std::cout  << std::endl;

    std::cout << "Remove the leaf 50: ";
    bool success = tree1.remove("50");
    check(success);

    std::cout << "Try to remove the leaf 50 again: ";
    success = tree1.remove("50");
    check(success);

    std::cout << "Remove the node 40 that has only a right child: ";
    success = tree1.remove("40");
    check(success);

    std::cout << "Tree 1 Preorder: Should be 10 20 30\n";
    tree1.preOrder(display);
    std::cout << "Tree 1 Inorder: Should be 10 20 30\n";
    tree1.inOrder(display);
    std::cout << "Tree 1 Postorder: Should be 30 20 10\n";
    tree1.postOrder(display);
    std::cout  << std::endl;

    std::cout << "Remove the root 10 (has a right subtree): ";
    success = tree1.remove("10");
    check(success);

    std::cout << "Tree 1 Preorder: Should be 20 30\n";
    tree1.preOrder(display);
    std::cout << "Tree 1 Inorder: Should be 20 30\n";
    tree1.inOrder(display);
    std::cout << "Tree 1 Postorder: Should be 30 20\n";
    tree1.postOrder(display);
    std::cout  << std::endl;


    // Part 2: Inserting data in random order
    BinarySearchTree<std::string> tree2(compare);

    tree2.insert("40");
    tree2.insert("20");
    tree2.insert("10");
    tree2.insert("60");
    tree2.insert("50");
    tree2.insert("70");
    tree2.insert("30");
    tree2.insert("80");

    //       40
    //     /     \
    //    20     60
    //   /  \   /  \
    //  10  30 50  70
    //               \
    //               80

    std::cout << "Tree 2 Preorder: Should be 40 20 10 30 60 50 70 80\n";
    tree2.preOrder(display);
    std::cout << "Tree 2 Inorder: Should be 10 20 30 40 50 60 70 80\n";
    tree2.inOrder(display);
    std::cout << "Tree 2 Postorder: Should be 10 30 20 50 80 70 60 40\n";
    tree2.postOrder(display);
    std::cout  << std::endl;

    std::cout << "Remove the node 70 that has only a right child: ";
    success = tree2.remove("70");
    check(success);
    //        40
    //     /     \
    //    20     60
    //   /  \   /  \
    //  10  30 50  80

    std::cout << "Tree 2 Inorder: Should be 10 20 30 40 50 60 80\n";
    tree2.inOrder(display);
    std::cout  << std::endl;

    std::cout << "Remove the node 60 that has two children: ";
    success = tree2.remove("60");
    check(success);
    //        40
    //     /     \
    //    20     80
    //   /  \   /
    //  10  30 50

    std::cout << "Tree 2 Inorder: Should be 10 20 30 40 50 80\n";
    tree2.inOrder(display);
    std::cout  << std::endl;

    std::cout << "Remove the leaf 10: ";
    success = tree2.remove("10");
    check(success);
    //        40
    //     /     \
    //    20     80
    //      \   /
    //      30 50

    std::cout << "Tree 2 Inorder: Should be 20 30 40 50 80\n";
    tree2.inOrder(display);
    std::cout  << std::endl;

    std::cout << "Remove the root 40: ";
    success = tree2.remove("40");
    check(success);
    //      50
    //     /  \
    //    20   80
    //      \
    //      30

    std::cout << "Tree 2 Inorder: Should be 20 30 50 80\n";
    tree2.inOrder(display);
    std::cout  << std::endl;

    std::string ret;
    auto ptr = tree2.getEntry("80");
    ret = ptr ? *ptr : "no entry";
    std::cout << "Searching for node 80 returns " << ret << std::endl;

    ptr = tree2.getEntry("40");
    ret = ptr ? *ptr : "no entry";
    std::cout << "Searching for node 40 returns " << ret << std::endl;



    // Part 3: Copying trees
    //at this point tree1 has only 20 and 30, so we're gonna reset it
    BinarySearchTree<std::string> newObj(compare);
    tree1 = newObj;
    tree1.insert("10");		// 10
    tree1.insert("20");		//	 |
    tree1.insert("30");		//    20
    tree1.insert("40");		//		|
    tree1.insert("50");		//		 30

    std::cout << "\nTesting copy constructor: \n";
    std::cout << "\nOriginal tree1: \n";
    std::cout << "Tree 1 Inorder: Should be 10 20 30 40 50\n";

    tree1.inOrder(display);
    BinarySearchTree<std::string> tree1Copy(tree1);
    std::cout << "Copy of Tree 1 Inorder: Should be 10 20 30 40 50\n";
    tree1Copy.inOrder(display);
    std::cout  << std::endl;

    std::cout << "\nTesting overloaded assignment operator: \n";
    std::cout << "\nAfter assigning Tree 1 to Tree 2, we have: \n";
    tree2 = tree1;

    std::cout << "Tree 2 Inorder: Should be 10 20 30 40 50\n";
    tree2.inOrder(display);
    std::cout  << std::endl;

    std::cout << "Change Tree 2 by removing 10" << std::endl;
    tree2.remove("10");

    std::cout << "Tree 2 Inorder: Should be 20 30 40 50\n";
    tree2.inOrder(display);
    std::cout << "Tree 1 Inorder: Should be 10 20 30 40 50\n";
    tree1.inOrder(display);
    std::cout << std::endl;

}

#endif //_BST_TEST