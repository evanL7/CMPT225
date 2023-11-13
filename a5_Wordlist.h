// Wordlist.h

#pragma once

/////////////////////////////////////////////////////////////////////////
//
// Student Info
// ------------
//
// Name : Evan Law
// St.# : 301464313
// Email: eel3@sfu.ca
//
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.
//
/////////////////////////////////////////////////////////////////////////

//
// Do not use any other #includes
//
#include "Wordlist_base.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

//
// IMPORTANT: no global variables are permitted in this file!
//
// It is okay to define helper functions defined outside the class.
//

class Wordlist : public Wordlist_base
{
    //
    // Use this Node to implement an AVL tree for the word list. You can *add*
    // extra variables/methods/features to this struct if you like, but you must
    // keep its name the same, and also keep the word, count, left, and right
    // variables as defined.
    //
    struct Node
    {
        string word;
        int count;
        Node *left;
        Node *right;

        int height; // Keeps track of the node's height
    };

    Node *root = nullptr;

    //
    // IMPORTANT: root is the only variable that can be defined in this class.
    // It should point to the top node of your AVL tree. When root == nullptr,
    // the tree is empty.
    //
    // No variables other than root are permitted!
    //

    // Default constructor
    Wordlist() {}


    // Constructor that accepts filename as the parameter
    Wordlist(const string& filename)
    {
        ifstream infile;
        infile.open(filename);
        if (infile.is_open()) // Checks that the provided file was opened correctly
        {
            string word;
            while (infile >> word)
            {
                add_word(word);
            }
            infile.close();
        }
    }


    // Destructor for Wordlist
    ~Wordlist()
    {
        freeTree(root);
        root = nullptr; // Prevent dangling pointers
    }


    // Helper function for the destructor that deallocates memory
    void freeTree(Node *n)
    {
        if (n != nullptr) // Uses recursion to delete the tree
        {
            freeTree(n->left);
            freeTree(n->right);
            free(n);
        }
    }


    // Helper function that searches through the tree using recursion and returns a node
    Node *search(Node *n, const string &w)
    {
        // Word does not exist in the tree or word exists
        if (n == nullptr || n->word == w) { return n; }
        if (n->word < w) { return search(n->right, w); } // Searches right half of the tree
        return search(n->left, w); // Searches left half of the tree
    }


    int get_count(const string &w) const
    { 
        Node *result = search(root, w);
        if (result == nullptr) { return 0; }
        return result->count;
    }


    int num_different_words() const
    {
        int total = 0;
        return num_nodes(root, total);
    }


    // Helper function for num_different_words method
    int num_nodes(Node *n, int &count)
    {
        if (n != nullptr)
        {
            num_nodes(n->left, count);
            num_nodes(n->right, count);
            count++;
        }
        return count;
    }


    int total_words() const
    {
        int total = 0;
        return num_nodes(root, total);
    }


    // Helper function for total_words method
    int tot_words(Node *n, int &count)
    {
        if (n != nullptr)
        {
            num_nodes(n->left, count);
            num_nodes(n->right, count);
            count += n->count;
        }
        return count;
    }


    bool is_sorted() const
    {
        return check_sorted(root, "a", "z");
    }


    // Helper function for is_sorted
    // Adapted from ChatGPT
    bool check_sorted(Node* n, const std::string& minVal = "", const std::string& maxVal = "")
    {
        if (n == nullptr) {
            return true; // An empty tree is considered sorted
        }

        if ((minVal != "" && n->word <= minVal) || (maxVal != "" && n->word >= maxVal)) 
        {
            return false; // The current node's value is outside the valid range
        }

        // Recursively check the left and right subtrees
        return check_sorted(n->left, minVal, n->word) 
            && check_sorted(n->right, n->word, maxVal);
    }


    string most_frequent() const
    {
        Node *max = find_max(root);
        return max->word + max->count;
    }


    // Helper function for most_frequent method
    Node *find_max(Node *n, Node *&max_node)
    {
        if (n == nullptr) { return; }
        
        find_max(n->left, max_node);

        if (max_node->count < n->count)
        {
            max_node = n;
        }
        
        find_max(n->right, max_node);
        return max_node;
    }


    int num_singletons() const
    {
        int total = 0;
        return count_singletons(root, total);
    }


    int count_singletons(Node *n, int &count)
    {
        if (n != nullptr)
        {
            num_nodes(n->left, count);
            num_nodes(n->right, count);

            if (n->count == 1) { count++; }
        }
        return count;
    }


    // Adapted from https://www.geeksforgeeks.org/insertion-in-an-avl-tree/
    void add_word(const string &w)
    {
        Node *new_root = insert(w);
        root = new_root;
    }


int height(Node *N)  
{  
    if (N == NULL)  
        return 0;  
    return N->height;  
}  
  
// A utility function to get maximum 
// of two integers  
int max(int a, int b)  
{  
    return (a > b)? a : b;  
}  
  
/* Helper function that allocates a  
   new node with the given key and  
   NULL left and right pointers. */
Node* create_node(string key)
{  
    Node *n = new Node(); 
    n->key = key;  
    n->left = NULL;  
    n->right = NULL;  
    n->height = 1; // new node is initially 
                      // added at leaf  
    return(n);  
}  
  
// A utility function to right 
// rotate subtree rooted with y  
// See the diagram given above.  
Node *rightRotate(Node *y)  
{  
    Node *x = y->left;  
    Node *T2 = x->right;  
  
    // Perform rotation  
    x->right = y;  
    y->left = T2;  
  
    // Update heights  
    y->height = max(height(y->left), 
                    height(y->right)) + 1;  
    x->height = max(height(x->left), 
                    height(x->right)) + 1;  
  
    // Return new root  
    return x;  
}  
  
// A utility function to left  
// rotate subtree rooted with x  
// See the diagram given above.  
Node *leftRotate(Node *x)  
{  
    Node *y = x->right;  
    Node *T2 = y->left;  
  
    // Perform rotation  
    y->left = x;  
    x->right = T2;  
  
    // Update heights  
    x->height = max(height(x->left),     
                    height(x->right)) + 1;  
    y->height = max(height(y->left),  
                    height(y->right)) + 1;  
  
    // Return new root  
    return y;  
}  
  
// Get Balance factor of node N  
int getBalance(Node *N)  
{  
    if (N == NULL)  
        return 0;  
    return height(N->left) - height(N->right);  
}  
  
// Recursive function to insert a key 
// in the subtree rooted with node and 
// returns the new root of the subtree.  
Node* insert(Node* node, string key)  
{  
    /* 1. Perform the normal BST insertion */
    if (node == NULL)  
        return(create_node(key));  
  
    if (key < node->key)  
        node->left = insert(node->left, key);  
    else if (key > node->key)  
        node->right = insert(node->right, key);  
    else // Equal keys are not allowed in BST  
        return node;  
  
    /* 2. Update height of this ancestor node */
    node->height = 1 + max(height(node->left),  
                        height(node->right));  
  
    /* 3. Get the balance factor of this ancestor  
        node to check whether this node became  
        unbalanced */
    int balance = getBalance(node);  
  
    // If this node becomes unbalanced, then  
    // there are 4 cases  
  
    // Left Left Case  
    if (balance > 1 && key < node->left->key)  
        return rightRotate(node);  
  
    // Right Right Case  
    if (balance < -1 && key > node->right->key)  
        return leftRotate(node);  
  
    // Left Right Case  
    if (balance > 1 && key > node->left->key)  
    {  
        node->left = leftRotate(node->left);  
        return rightRotate(node);  
    }  
  
    // Right Left Case  
    if (balance < -1 && key < node->right->key)  
    {  
        node->right = rightRotate(node->right);  
        return leftRotate(node);  
    }  
  
    /* return the (unchanged) node pointer */
    return node;  
}  


}; // class Wordlist
