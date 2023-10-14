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

    }

    int size() const
    {
        return sz;
    }

    void enqueue(const Announcement &item)
    {
        if (left == nullptr) // Empty doubly linked list
        {
            Node *new_node = new Node{item, nullptr, nullptr};
            left = new_node;
            right = new_node;
            return;
        }
        
    }


    Announcement &front() const
    {
        return left->data;
    }


    void dequeue()
    {
        return;
    }


    void print_queue()
    {
        while (left != nullptr)
        {
            cout << left->data << endl;

            cout << endl;
            left = left->next;
        }
    }

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
        num_lines++;

        if (line.find("SEND") != std::string::npos && num_lines == 1)
        {
            start_pos = std::string("SEND ").size();
            parsed_text = line.substr(start_pos);
            //cout << "TEXT:::" << parsed_text << endl;

            Announcement text(parsed_text);
            //cout << "WHAT IS THIS" << endl << text << "\nLOOOK UP\n";
            q.enqueue(text);
        }
        else if (line.find("REMOVE_ALL") != std::string::npos)
        {

        }
        else if (line.find("PROMOTE_ANNOUNCEMENTS") != std::string::npos)
        {

        }
        else if (line.find("ANNOUNCE") != std::string::npos)
        {

        }



        cout << "line " << num_lines << ": " << line << endl;
    }

    cout << endl << endl;
    q.print_queue();

    return 0;
}

