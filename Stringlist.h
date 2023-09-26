// Stringlist.h

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
// Do not use any other #includes or #pragmas in this file.
//

#pragma once

#include <cassert>
#include <iostream>
#include <string>

using namespace std;

class Stringlist
{

    // Nodes will create a singly-linked list
    struct Node
    {
        // Specifies operation to do formatted in the form:
        // "[OPERATION] [INDEX] [special cases: word/list]"
        string operation; 
        Node *next;
    };

    struct MyStack
    {
        Node *head;
    };

    MyStack *undo_stack = new MyStack{nullptr}; // Holds the actions to perform

    int cap;     // capacity
    string *arr; // array of strings
    int sz;      // size

    //
    // Helper function for throwing out_of_range exceptions.
    //
    void bounds_error(const string &s) const
    {
        throw out_of_range("Stringlist::" + s + " index out of bounds");
    }

    //
    // Helper function for checking index bounds.
    //
    void check_bounds(const string &s, int i) const
    {
        if (i < 0 || i >= sz)
        {
            bounds_error(s);
        }
    }

    //
    // Helper function for copying another array of strings.
    //
    void copy(const string *other)
    {
        for (int i = 0; i < sz; i++)
        {
            arr[i] = other[i];
        }
    }

    //
    // Helper function for checking capacity; doubles size of the underlying
    // array if necessary.
    //
    void check_capacity()
    {
        if (sz == cap)
        {
            cap *= 2;
            string *temp = new string[cap];
            for (int i = 0; i < sz; i++)
            {
                temp[i] = arr[i];
            }
            delete[] arr;
            arr = temp;
        }
    }


public:
    //
    // Default constructor: makes an empty StringList.
    //
    Stringlist()
        : cap(10), arr(new string[cap]), sz(0)
    {
    }

    //
    // Copy constructor: makes a copy of the given StringList.
    //
    // Does *not* copy the undo stack, or any undo information from other.
    //
    Stringlist(const Stringlist &other)
        : cap(other.cap), arr(new string[cap]), sz(other.sz)
    {
        copy(other.arr);
    }

    //
    // destructor
    //
    ~Stringlist()
    {
        delete[] arr;        
        delete_stack(undo_stack); // Deletes the undo stack
    }


    //
    // Assignment operator: makes a copy of the given StringList.
    //
    // undoable
    //
    // For undoing, when assigning different lists, the undo stack is NOT
    // copied:
    //
    //    lst1 = lst2; // lst1 undo stack is updated to be able to undo the
    //                 // assignment; lst1 does not copy lst2's stack
    //                 //
    //                 // lst2 is not change in any way
    //
    // Self-assignment is when you assign a list to itself:
    //
    //    lst1 = lst1;
    //
    // In this case, nothing happens to lst1. Both its string data and undo
    // stack are left as-is.
    //
    Stringlist &operator=(const Stringlist &other)
    {
        if (this != &other)
        {
            // Form: "set list to {word, word, ... , word}"
            string operation = "set list to " + this->to_string();
            push(undo_stack, operation);

            delete[] arr;
            cap = other.capacity();
            arr = new string[cap];
            sz = other.size();
            copy(other.arr);
        }
        return *this;
    }

    //
    // Returns the number of strings in the list.
    //
    int size() const { return sz; }

    //
    // Returns true if the list is empty, false otherwise.
    //
    bool empty() const { return size() == 0; }

    //
    // Returns the capacity of the list, i.e. the size of the underlying array.
    //
    int capacity() const { return cap; }

    //
    // Returns the string at the given index.
    //
    string get(int index) const
    {
        check_bounds("get", index);
        return arr[index];
    }

    //
    // Returns the index of the first occurrence of s in the list, or -1 if s is
    // not in the lst.
    //
    int index_of(const string &s) const
    {
        for (int i = 0; i < sz; i++)
        {
            if (arr[i] == s)
            {
                return i;
            }
        }
        return -1;
    }

    //
    // Returns true if s is in the list, false otherwise.
    //
    bool contains(const string &s) const
    {
        return index_of(s) != -1;
    }

    //
    // Returns a string representation of the list.
    //
    string to_string() const
    {
        string result = "{";
        for (int i = 0; i < size(); i++)
        {
            if (i > 0)
            {
                result += ", ";
            }
            result += "\"" + get(i) + "\"";
        }
        return result + "}";
    }

    //
    // Sets the string at the given index.
    //
    // undoable
    //
    void set(int index, string value)
    {
        // Get the word to be removed to push to the undo stack
        string prev_word = Stringlist::get(index);

        check_bounds("set", index);
        arr[index] = value;

        // Concatenates the operation and uses the to_string function to convert int to str
        // Form: "SET # [word]"
        string operation = "SET " + std::to_string(index) + " " + prev_word;
        push(undo_stack, operation);
    }

    //
    // Insert s before index; if necessary, the capacity of the underlying array
    // is doubled.
    //
    // undoable
    //
    void insert_before(int index, const string &s)
    {
        if (index < 0 || index > sz)
        { // allows insert at end, i == sz
            bounds_error("insert_before");
        }
        check_capacity();

        for (int i = sz; i > index; i--)
        {
            arr[i] = arr[i - 1];
        }
        arr[index] = s;
        sz++;

        // Concatenates the operation and uses the to_string function to convert int to str
        string operation = "REMOVE " + std::to_string(index); // Form: "REMOVE #"
        push(undo_stack, operation);
    }

    //
    // Appends s to the end of the list; if necessary, the capacity of the
    // underlying array is doubled.
    //
    // undoable
    //
    void insert_back(const string &s)
    {
        insert_before(size(), s);
    }

    //
    // Inserts s at the front of the list; if necessary, the capacity of the
    // underlying array is doubled.
    //
    // undoable
    //
    void insert_front(const string &s)
    {
        insert_before(0, s);
    }

    //
    // Removes the string at the given index; doesn't change the capacity.
    //
    // undoable
    //
    void remove_at(int index)
    {
        // Get the word to be removed to push to the undo stack
        string prev_word = Stringlist::get(index);

        check_bounds("remove_at", index);
        for (int i = index; i < sz - 1; i++)
        {
            arr[i] = arr[i + 1];
        }
        sz--;

        // Concatenates the operation and uses the to_string function to convert int to str
        // Form: "INSERT # [word]"
        string operation = "INSERT " + std::to_string(index) + " " + prev_word;
        push(undo_stack, operation);
    }

    //
    // Removes all strings from the list; doesn't change the capacity.
    //
    // undoable
    //
    void remove_all()
    {
        // Form: "set list to {word, word, ... , word}"
        string operation = "set list to " + to_string();
        push(undo_stack, operation);

        int nodes_to_delete = sz; // Get the size of the list before deletion

        while (sz > 0)
        {
            remove_at(sz - 1);
        }

        // Since remove_at function call adds nodes to the stack,
        // need to remove the added nodes from the stack to get back to the state before
        // the while loop above
        Node *to_delete = undo_stack->head;
        while (nodes_to_delete != 0)
        {
            undo_stack->head = undo_stack->head->next;
            delete to_delete;
            to_delete = undo_stack->head;
            nodes_to_delete--;
        }
    }

    //
    // Removes the first occurrence of s in the list, and returns true. If s is
    // nowhere in the list, nothing is removed and false is returned.
    //
    // undoable
    //
    bool remove_first(const string &s)
    {
        int index = index_of(s);
        if (index == -1)
        {
            return false;
        }
        remove_at(index);
        return true;
    }

    //
    // Undoes the last operation that modified the list. Returns true if a
    // change was undone.
    //
    // If there is nothing to undo, does nothing and returns false.
    //
    bool undo()
    {
        if (undo_stack != nullptr) // Undo stack is not empty
        {
            string operation = undo_stack->head->operation; // Get the operation to perform
            int start_pos, index_pos; // Find where number begins, store what the number is

            // Parses the string to identify the operation to perform by
            // using the find method, npos means no match
            if (operation.find("REMOVE") != std::string::npos) // Form: "REMOVE #"
            {
                start_pos = std::string("REMOVE ").size(); 
                
                // Get the number as a string and then
                // convert the number from a string to an integer
                index_pos = std::stoi(operation.substr(start_pos)); 

                for (int i = index_pos, swaps = sz - 1; i < swaps; i++)
                {
                    arr[i] = arr[i + 1]; // Shifts the elements to the left
                }
                sz--; // Decrements the number of words in the list
            }
            else if (operation.find("SET") != std::string::npos) // Form: "SET # [word]"
            {
                start_pos = std::string("SET ").size();

                // Finds the space character after the number to parse the number
                int second_space = operation.find(" ", start_pos);
                index_pos = std::stoi(operation.substr(start_pos, second_space - start_pos)); 
                
                // Parses the word to put back
                string word = operation.substr(second_space + 1);

                arr[index_pos] = word; // Reverts the word back to original
            }
            else if (operation.find("INSERT") != std::string::npos) // Form: "INSERT # [word]"
            {
                start_pos = std::string("INSERT ").size();

                // Finds the space character after the number to parse the number
                int second_space = operation.find(" ", start_pos);
                index_pos = std::stoi(operation.substr(start_pos, second_space - start_pos)); 
                
                // Parses the word to put back
                string word = operation.substr(second_space + 1);

                string current = arr[index_pos]; // Gets the existing word in the list
                arr[index_pos] = word; // Inserts the word back

                string next_word; // Temporary variable

                // Shifts the words to the right to revert the changes
                for (int i = index_pos, swaps = sz - 1; i < swaps; i++)
                {
                    next_word = arr[i+1]; 
                    arr[i+1] = current;
                    current = next_word;
                }
                sz++; // Increment the number of words in the list
            }
            else if (operation.find("set list") != std::string::npos)
            {   // Form: "set list to {word, word, ... , word}"

                // Count variable keeps track of the index location to start searching 
                // for the quotation character, prev_sz keeps track of the size of the list,
                // first and second quote variables look for 
                // the start of the word and end of the word, respectively
                int count = 0, prev_sz = 0, first_quote, second_quote;
                string word; // Parses the word to put back

                // Reconstructs the previous word list by parsing the words one at a time
                while (true)
                {
                    // Assumes no words in the list already have quotes in the word
                    first_quote = operation.find("\"", count);
                    if (first_quote == std::string::npos)
                    {
                        break; // No more words left in the original list
                    }
                    second_quote = operation.find("\"", first_quote + 1);
                    
                    word = operation.substr(first_quote + 1, 
                                            second_quote - first_quote - 1);
                    arr[prev_sz] = word;
                    count = first_quote + 1 + (second_quote - first_quote);
                    prev_sz++;
                }
                sz = prev_sz;
            }

            // Removes the node from the stack
            Node *to_delete = undo_stack->head;
            undo_stack->head = undo_stack->head->next;
            delete to_delete;

            return true;
        }
        return false;
    }


    // Pushes an operation to the undo stack
    void push(MyStack *s, string operation)
    {
        Node *new_node = new Node{operation, nullptr};
        if (s->head == nullptr) // Currently an empty stack
        {
            s->head = new_node;
        }
        else // Not an empty stack
        {
            new_node->next = s->head;
            s->head = new_node;
        }
    }

    // Helper function that deletes the undo stack
    void delete_stack(MyStack *s)
    {
        Node *to_delete = s->head;
        while (s->head != nullptr)
        {
            s->head = s->head->next;
            delete to_delete;
            to_delete = s->head;
        }
        s->head = nullptr; // Prevent dangling pointer
        delete undo_stack;
    }


}; // class Stringlist

//
// Prints list to in the format {"a", "b", "c"}.
//
ostream &operator<<(ostream &os, const Stringlist &lst)
{
    return os << lst.to_string() << endl;
}

//
// Returns true if the two lists are equal, false otherwise.
//
// Does *not* consider any undo information when comparing two Stringlists. All
// that matters is that they have the same strings in the same order.
//
bool operator==(const Stringlist &a, const Stringlist &b)
{
    if (a.size() != b.size())
    {
        return false;
    }
    for (int i = 0; i < a.size(); i++)
    {
        if (a.get(i) != b.get(i))
        {
            return false;
        }
    }
    return true;
}

//
// Returns true if the two lists are not equal, false otherwise.
//
// Does *not* consider any undo information when comparing two Stringlists.
//
bool operator!=(const Stringlist &a, const Stringlist &b)
{
    return !(a == b);
}
