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


// This program creates a Wordlist class that stores words without any duplicates. 
// The class is implemented using an AVL tree data structure.


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
        int height; // Keeps track of the node's height to maintain the AVL tree
        Node *left;
        Node *right;
    };

    Node *root = nullptr;

    //
    // IMPORTANT: root is the only variable that can be defined in this class.
    // It should point to the top node of your AVL tree. When root == nullptr,
    // the tree is empty.
    //
    // No variables other than root are permitted!
    //

public:

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
            while (infile >> word) {add_word(word);}
            infile.close();
        }
    }


    // Destructor for Wordlist
    ~Wordlist() {freeTree(root);}


    // Helper recursive function for the destructor that deallocates memory
    void freeTree(Node *n)
    {
        if (n != nullptr)
        {
            freeTree(n->left);
            freeTree(n->right);
            delete n;
            n = nullptr; // Prevent dangling pointers
        }
    }


    int get_count(const string &w) const
    { 
        Node *result = get_count_rec(root, w);
        if (result == nullptr) {return 0;} // Case where word does not exist
        return result->count;
    }


    // Helper recursive function for get_count method
    // that searches through the tree using recursion and returns a node
    // Adapted from https://www.geeksforgeeks.org/binary-search-tree-set-1-search-and-insertion/
    Node *get_count_rec(Node *n, const string &w) const
    {
        // Word does not exist in the tree or word exists
        if (n == nullptr || n->word == w) {return n;}
        if (n->word < w) {return get_count_rec(n->right, w);} // Searches right half of the tree
        return get_count_rec(n->left, w); // Searches left half of the tree
    }


    int num_different_words() const {return num_different_words_rec(root);}


    // Helper recursive function for num_different_words method
    int num_different_words_rec(Node *n) const
    {
        if (n == nullptr) {return 0;}
        return 1 + num_different_words_rec(n->left) + num_different_words_rec(n->right);
    }


    int total_words() const {return total_words_rec(root);}


    // Helper recursive function for total_words method
    int total_words_rec(Node *n) const
    {
        if (n == nullptr) {return 0;}
        return n->count + total_words_rec(n->left) + total_words_rec(n->right);
    }


    bool is_sorted() const
    {
        Node *prev = nullptr;
        return is_sorted_rec(root, prev);
    }


    // Helper recursive function for is_sorted method
    // Adapted from ChatGPT
    bool is_sorted_rec(Node *n, Node *&prev) const
    {
        if (n == nullptr) {return true;}

        if (!is_sorted_rec(n->left, prev)) {return false;} // Check the left subtree
        if (prev != nullptr && prev->word > n->word) {return false;} // Check the current node

        prev = n;

        return is_sorted_rec(n->right, prev); // Check the right subtree
    }


    string most_frequent() const
    {
        Node *current_max = root;
        Node *max = most_frequent_rec(root, current_max);
        return max->word + " " + std::to_string(max->count);
    }


    // Helper recursive function for most_frequent method
    // Adapted from ChatGPT
    Node *most_frequent_rec(Node *n, Node *&max_node) const
    {
        if (n == nullptr) {return nullptr;}

        Node *left_max = most_frequent_rec(n->left, max_node);
        Node *right_max = most_frequent_rec(n->right, max_node);

        // Compare left subtree max
        if (left_max != nullptr && left_max->count > n->count) {max_node = left_max;}

        // Compare right subtree max
        if (right_max != nullptr && right_max->count > n->count) {max_node = right_max;}

        // Compare current node with the overall max
        if (max_node->count < n->count || 
        (max_node->count == n->count && n->word < max_node->word)) {max_node = n;}

        return max_node;
    }


    int num_singletons() const
    {
        int total = 0;
        total = num_singletons_rec(root, total);
        return total;
    }


    // Helper recursive function for num_singletons method
    // that traverses nodes in postorder traversal
    // Adapted from https://www.geeksforgeeks.org/tree-traversals-inorder-preorder-and-postorder/
    int num_singletons_rec(Node *n, int &count) const
    {
        if (n != nullptr)
        {
            num_singletons_rec(n->left, count);
            num_singletons_rec(n->right, count);
            if (n->count == 1) {count++;}
        }
        return count;
    }


    // Adapted from https://www.geeksforgeeks.org/insertion-in-an-avl-tree/
    void add_word(const string &w)
    {
        Node *new_root = insert(root, w);
        root = new_root;
    }


    // Helper recursive function for add_word method
    // First inserts the node into the binary search tree
    // then updates the inserted node's height
    // then checks if the balance factor requires rotations and applies rotations if necessary
    // returns the new root
    // Adapted from https://www.geeksforgeeks.org/insertion-in-an-avl-tree/
    Node *insert(Node* n, string key)  
    {  
        // Inserts the node into the tree
        if (n == nullptr) {return create_node(key);}
    
        if (key < n->word) {n->left = insert(n->left, key);} // Moves to the left              
        else if (key > n->word) {n->right = insert(n->right, key);} // Moves to the right

        else // Duplicate keys increment the existing node's count and do not get inserted 
        {
            n->count++;
            return n;
        }


        // Updates the height of the inserted node
        n->height = 1 + max(height(n->left), height(n->right));
                              
    
        // Gets the balance factor of the inserted node to check if rotations are necessary
        int balance = getBalance(n);  
    
        // 4 cases to check if the node is unbalanced

        // Left Left case  
        if (balance > 1 && key < n->left->word) {return rightRotate(n);}
    
        // Right Right case
        if (balance < -1 && key > n->right->word) {return leftRotate(n);}
    
        // Left Right case
        if (balance > 1 && key > n->left->word)
        {
            n->left = leftRotate(n->left);
            return rightRotate(n);
        }
    
        // Right Left case
        if (balance < -1 && key < n->right->word)  
        {  
            n->right = rightRotate(n->right);  
            return leftRotate(n);  
        }  
    
        return n; // If the inserted node still maintains the AVL tree, nothing done
    }


    // Helper function for insert function that allocates memory for a node in the tree
    // count = 1 because creating a node adds a word to the count
    // height = 1 because new node is initially added at leaf
    // Adapted from https://www.geeksforgeeks.org/insertion-in-an-avl-tree/
    Node* create_node(string key) {return new Node{key, 1, 1, nullptr, nullptr};}


    // Helper function for insert function that determines the bigger number
    // Adapted from https://www.geeksforgeeks.org/insertion-in-an-avl-tree/
    int max(int a, int b)
    {  
        if (a > b) {return a;}
        return b;
    }  


    // Helper function for insert function that returns the passed in node's height
    // Adapted from https://www.geeksforgeeks.org/insertion-in-an-avl-tree/
    int height(Node *n)  
    {  
        if (n == nullptr) {return 0;}
        return n->height;
    } 


    // Helper function for insert function that gets the balance factor by 
    // subtracting the left node's height by the right node's height
    // Adapted from https://www.geeksforgeeks.org/insertion-in-an-avl-tree/
    int getBalance(Node *n)  
    {  
        if (n == nullptr) {return 0;}
        return height(n->left) - height(n->right);
    }  

    
    // Helper function for insert function
    // that rotates the subtree to the right, rooted at y and maintains the AVL tree
    /*
    Visual Illustration of rightRotate(y) Function

    Before:    y       After:   x
              /                /\
             x   -->   unchanged y
            /\                  /    
    unchanged subT            subT

    */
    // Adapted from https://www.geeksforgeeks.org/insertion-in-an-avl-tree/
    Node *rightRotate(Node *y)  
    {  
        Node *x = y->left;
        Node *subT = x->right;
    
        // Perform rotation
        x->right = y;
        y->left = subT;
    
        // Update heights
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;    
        
        return x; // Return new root
    }  
    

    // Helper function for insert function
    // that rotates the subtree to the left, rooted at x and maintains the AVL tree
    /*
    Visual Illustration of leftRotate(x) Function

    Before:  x         After:   y
              \                /\
               y     -->      x unchanged
               /\              \    
            subT unchanged     subT

    */
    // Adapted from https://www.geeksforgeeks.org/insertion-in-an-avl-tree/
    Node *leftRotate(Node *x)  
    {  
        Node *y = x->right;
        Node *subT = y->left;
    
        // Perform rotation
        y->left = x;
        x->right = subT;

        // Update heights
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y; // Return new root
    }  


    void print_words() const
    {
        int count = 1;
        print_words_rec(root, count);
        cout << endl; // New line to match text file output from assignment
    }


    // Helper recursive function for print_words method
    // that traverses nodes in inorder traversal
    // Adapted from https://www.geeksforgeeks.org/tree-traversals-inorder-preorder-and-postorder/
    void print_words_rec(Node *n, int &count) const
    {
        if (n != nullptr)
        {
            print_words_rec(n->left, count);
            cout << count << ". " << "{\"" << n->word << "\", " << n->count << "}" << endl;
            count++;
            print_words_rec(n->right, count);
        }        
    }

}; // class Wordlist
