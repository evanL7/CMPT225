// a3.cpp

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
// Do not #include any other files!
//
#include "Announcement.h"
#include "JingleNet_announcer.h"
#include "Queue_base.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;


class Queue : public Queue_base<Announcement>
{

    // Nodes create a doubly linked list
    struct Node
    {
        Announcement data;
        Node *next;
        Node *prev;
    };

    int sz = 0; // Keeps track of the number of nodes
    
    Node *left = nullptr; // Pointer to the front of the queue
    Node *right = nullptr; // Pointer to the back of the queue


public:

    // Destructor
    ~Queue()
    {
        Node *to_delete;
        while (left != nullptr)
        {
            to_delete = left;
            left = left->next;
            delete to_delete;
        }
        left = nullptr; // Prevent dangling pointers
        right = nullptr;
    }


    int size() const
    {
        return sz;
    }

    void enqueue(const Announcement &item)
    {
        Node *new_node = new Node{item, nullptr, nullptr};
        if (right == nullptr) // Empty doubly linked list
        {
            left = new_node;
            right = new_node;
        }
        else if (right->prev == nullptr) // Single element case
        {
            new_node->prev = right;
            right->next = new_node;
            right = new_node;            
        }
        else // Not empty
        {
            // Rework the pointers to maintain the doubly linked list
            right->next = new_node;
            new_node->prev = right;
            right = new_node;
        }
        sz++; // Increment the number of nodes
    }


    Announcement &front() const
    {
        if (left == nullptr) // Empty doubly linked list
        {
            throw runtime_error("front: queue is empty");
        }
        return left->data;
    }


    void dequeue()
    {
        if (left == nullptr) // Empty doubly linked list
        {
            throw runtime_error("dequeue: queue is empty");
        }
        else if (left->next == nullptr) // Single element case
        {
            delete left;
            left = nullptr; // Prevent dangling pointers
            right = nullptr;
        }
        else // Not empty
        {
            // Rework the pointers to maintain the doubly linked list
            Node *to_delete = left;
            left = left->next;
            left->prev = nullptr;
            delete to_delete;
        }
        sz--; // Decrement the number of nodes
    }


    // DELETE AFTER: prints starting from the front of the queue
    void print_queue()
    {
        while (left != nullptr)
        {
            cout << left->data << endl;
            left = left->next;
        }
    }

};


class JingleNet : public Queue
{
    Queue system;

    Queue santa;
    Queue reindeer;
    Queue elf2;
    Queue elf1;
    Queue snowman;
};


int main(int argc, char *argv[])
{
    cout << "Welcome to Assignment 3!" << endl;

    // Check that the user provided a filename.
    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return 1; 
    }

    string filename = argv[1];
    cout << "Printing " << filename << " ..." << endl;
    ifstream infile(filename);
    string line;
    int num_lines = 0;

    Queue q;
    int start_pos;
    string parsed_text;

    while (getline(infile, line))
    {

        if (line.substr(0,4) == "SEND")
        {
            start_pos = std::string("SEND ").size();
            parsed_text = line.substr(start_pos);

            Announcement text(parsed_text);
            q.enqueue(text);
        }
        else if (line.substr(0,10) == "REMOVE_ALL")
        {
        }
        else if (line.substr(0,21) == "PROMOTE_ANNOUNCEMENTS")
        {
        }
        else if (line.substr(0,8) == "ANNOUNCE")
        {
        }


        num_lines++;

        cout << "line " << num_lines << ": " << line << endl;
    }

    cout << endl;
    q.print_queue();

    return 0;
}

