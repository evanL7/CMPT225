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

#include "Wordlist_base.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class Wordlist : public Wordlist_base
{
    // Node creates a doubly-linked list that stores the words
    struct Node
    {
        string word;
        Node *next;
        Node *prev;
    };

    Node *head; // Holds the first node of the doubly-linked list
    bool frozen; // Specifies whether the doubly-linked list is frozen or not
    int size = 0; // Tracks the number of nodes in the doubly-linked list

public:

    // Default constructor that uses initialization lists for efficiency
    // frozen variable is initially false until the get_sorted_index() method is called
    Wordlist()
        : head(nullptr), frozen(false)
    {}


    // Copy constructor. Note that the copy constructor copies the boolean value of frozen
    Wordlist(const Wordlist& other)
        : head(nullptr), frozen(other.frozen)
    {
        Node *current = other.head;
        while (current != nullptr) // Loops until the end of the doubly-linked list
        {
            add_word(current->word);
            current = current->next;
        }
    }
    

    // Constructor that accepts filename as the parameter
    Wordlist(const string& filename)
        : head(nullptr), frozen(false)
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


    // Destructor that deallocates memory for the doubly-linked list
    ~Wordlist()
    {
        Node *to_delete; // Stores the node to be deleted
        while (head != nullptr)
        {
            to_delete = head;
            head = head->next;
            delete to_delete;
        }
        head = nullptr; // Prevent dangling pointers
    }


    bool is_frozen() const
    {
        return frozen;
    }


    bool contains(const string &w) const
    {
        Node *current = head;
        while (current != nullptr)
        {
            if (w == current->word)
            {
                return true; // w is in the word list
            }
            current = current->next;
        }
        return false; // Exiting the loop implies word not found
    }


    int length() const
    {
        return size;
    }


    string get_word(int index) const
    {
        Node *current = head;
        int count = 0;
        while (index != count) // Loop until specified index is reached
        {
            current = current->next;
            count++;
        }
        return current->word;
    }


    void add_word(const string &w)
    {
        if (is_frozen())
        {
            throw runtime_error("frozen list: cannot add word because "
                                "get_sorted_index() method has already been called");
        }

        Node *new_node = create_node(w);

        if (head == nullptr) // Currently an empty doubly-linked list
        {
            head = new_node;
        }
        else if (head->next == nullptr) // Only one element in the doubly-linked list
        {            
            if (w == head->word) // Checks if the word already exists in the doubly-linked list
            {
                delete new_node; // Free the memory that was allocated
                size--;
                return;
            }
            head->next = new_node;
            new_node->prev = head;
        }
        else
        {
            Node *current = head;
            while (current->next != nullptr) // Keep iterating until last node is reached
            {
                if (w == current->word) // Do nothing if w is already in the list
                {
                    delete new_node; // Free the memory that was allocated
                    size--;
                    return;
                }
                current = current->next;
            }
            // Condition applies to solely check the last node in the doubly-linked list
            // so that if the word doesn't already exist in the doubly-linked list, 
            // the word can be inserted
            if (w == current->word) // Do nothing if w is already in the list
            {
                delete new_node; // Free the memory that was allocated
                size--;
                return;
            }
            current->next = new_node;
            new_node->prev = current;
        }
    }


    // Removes specified word
    void remove_word(const string &w)
    {
        if (is_frozen())
        {
            throw runtime_error("frozen list: cannot remove word because "
                                "get_sorted_index() method has already been called");
        }

        Node *current = head;

        while (current != nullptr && w != current->word) // Get to the node to be removed
        {
            current = current->next;
        }
        if (current != nullptr) // Check that the word exists in the doubly-linked list
        {
            // Rework the pointers to maintain the doubly-linked list
            if (current->next != nullptr)
            {
                current->next->prev = current->prev;
            }
            if (current->prev != nullptr)
            {
                current->prev->next = current->next;
            }
            if (current == head)
            {
                head = current->next;
            }
            delete current;
            size--; // Decrement the number of nodes in the list
        }
    }


    vector<string *> get_sorted_index()
    {
        vector<string *> words;
        Node *current = head;
        while (current != nullptr) // Insert all the words in the doubly-linked list to the vector
        {
            words.push_back(&(current->word));
            current = current->next;
        }

        int i, j;
        int vector_length = words.size();
        string *str1, *str2;

        // Uses insertion sort to sort the words alphabetically and 
        // compares the strings using the compare method
        for (i = 1; i < vector_length; i++)
        {
            j = i;
            str1 = words[j];
            while (j > 0)
            {
                str2 = words[j-1]; // Stores words before str1
                if (str1->compare(*str2) < 0) // Check if str1 needs to be shifted to the left
                {
                    swap(words[j], words[j-1]);
                }               
                j--;
            }
        }        
        frozen = true; // Freezes the word list
        return words;
    }


    // Helper function that creates a node for the doubly linked list
    Node *create_node(const string word)
    {
        // Increments the number of nodes in the doubly-linked list as they're created
        // and decrements the size if the word associated with the node already exists
        size++;
        return new Node{word, nullptr, nullptr};
    }

}; // class Wordlist
