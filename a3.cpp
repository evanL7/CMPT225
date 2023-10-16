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
        Node *current = left;

        while (current != nullptr)
        {
            cout << current->data << endl;
            current = current->next;
        }
    }

};


class JingleNet : public Queue
{
    Queue snowman;
    Queue elf1;
    Queue elf2;
    Queue reindeer;
    Queue santa;

public:

    // Default constructor
    JingleNet()
    {}


    // SEND command that adds announcement to the specified rank
    void send(string line)
    {
        Announcement text(line);
        string text_rank = to_string(text.get_rank());

        if (text_rank == "snowman") { snowman.enqueue(text); }
        else if (text_rank == "elf1") { elf1.enqueue(text); }
        else if (text_rank == "elf2") { elf2.enqueue(text); }
        else if (text_rank == "reindeer") { reindeer.enqueue(text); }
        else if (text_rank == "santa") { santa.enqueue(text); }
    }
    

    // REMOVE_ALL command that removes specified name
    void remove_sender(string name)
    {
        if (snowman.size() > 0) { to_remove(name, snowman); }
        if (elf1.size() > 0) { to_remove(name, elf1); }
        if (elf2.size() > 0) { to_remove(name, elf2); }
        if (reindeer.size() > 0) { to_remove(name, reindeer); }
        if (santa.size() > 0) { to_remove(name, santa); }
    }


    // Helper function that removes specified name from a given queue and size
    void to_remove(string username, Queue &q)
    {
        int sz = q.size();
        while (sz > 0) // Loops to check each element
        {
            Announcement current(q.front());
            q.dequeue();

            // Adds element to back to the queue if the name does not match
            if (current.get_sender_name() != username) { q.enqueue(current); }
            sz--;
        }
    }
    

    // PROMOTE_ANNOUNCEMENTS command that promotes specified user
    void promote_user(string name)
    {
        if (reindeer.size() > 0) { to_promote(name, reindeer, santa, "santa"); }
        if (elf2.size() > 0) { to_promote(name, elf2, reindeer, "reindeer"); }
        if (elf1.size() > 0) { to_promote(name, elf1, elf2, "elf2"); }
        if (snowman.size() > 0) { to_promote(name, snowman, elf1, "elf1"); }
    }


    // Helper function that promotes specified name from a given queue
    // and moves the node to the next rank
    void to_promote(string username, Queue &q_start, Queue &q_end, string new_rank)
    {
        int q_start_sz = q_start.size();
        while (q_start_sz > 0) // Loops to check each element
        {
            Announcement current(q_start.front());
            q_start.dequeue();

            // Adds element to the queue of a rank one higher if the name matches
            if (current.get_sender_name() == username)
            {
                // Updates the announcement's rank to the higher rank
                Announcement new_current(current.get_sender_name(), 
                                         to_rank(new_rank), 
                                         current.get_text());
                q_end.enqueue(new_current);
            }
            else { q_start.enqueue(current); } // Otherwise put the element back
            q_start_sz--;
        }
    }


    // ANNOUNCE command that announces n announcements from highest priority to lowest priority
    void to_announce(int n)
    {
        int msgs_to_send = n;
        if (santa.size() > 0) { msgs_to_send = send_announcement(msgs_to_send, santa); }
        if (reindeer.size() > 0) { msgs_to_send = send_announcement(msgs_to_send, reindeer); }
        if (elf2.size() > 0) { msgs_to_send = send_announcement(msgs_to_send, elf2); }
        if (elf1.size() > 0) { msgs_to_send = send_announcement(msgs_to_send, elf1); }
        if (snowman.size() > 0) { msgs_to_send = send_announcement(msgs_to_send, snowman); }
    }


    // Helper function that passes the number of messages to left to send
    // and the queue to be dequeued
    int send_announcement(int msgs_left, Queue &q)
    {
        int sz = q.size();
        while (msgs_left > 0 && sz > 0)
        {
            Announcement a(q.front());
            q.dequeue();
            jnet.announce(a);
            msgs_left--;
            sz--;
        }
        return msgs_left;
    }

    // DELETE AFTER: prints out all the queues
    void printAllQueues()
    {
        cout << "santa:\n";
        santa.print_queue();

        cout << "reindeer:\n";
        reindeer.print_queue();

        cout << "elf2:\n";
        elf2.print_queue();

        cout << "elf1:\n";
        elf1.print_queue();

        cout << "snowman:\n";
        snowman.print_queue();
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

    JingleNet system;

    int start_pos;
    string parsed_text;

    while (getline(infile, line))
    {

        if (line.substr(0,4) == "SEND")
        {
            start_pos = std::string("SEND ").size();
            parsed_text = line.substr(start_pos); // Read only the contents after the command
            system.send(parsed_text);
        }
        else if (line.substr(0,10) == "REMOVE_ALL")
        {
            //cout << "\n\n\nBefore remove_all:" << endl;
            //system.printAllQueues();

            start_pos = std::string("REMOVE_ALL ").size();
            parsed_text = line.substr(start_pos); // Read only the contents after the command
            system.remove_sender(parsed_text);
        }
        else if (line.substr(0,21) == "PROMOTE_ANNOUNCEMENTS")
        {
            //cout << "\n\n\nBefore PROMOTE_ANNOUNCEMENTS:" << endl;
            //system.printAllQueues();


            start_pos = std::string("PROMOTE_ANNOUNCEMENTS ").size();
            parsed_text = line.substr(start_pos); // Read only the contents after the command
            system.promote_user(parsed_text);
        }
        else if (line.substr(0,8) == "ANNOUNCE")
        {
            //cout << "\n\n\nBefore ANNOUNCE:" << endl;
            //system.printAllQueues();


            start_pos = std::string("ANNOUNCE ").size();
            parsed_text = line.substr(start_pos); // Read only the contents after the command
            system.to_announce(std::stoi(parsed_text)); // atoi function converts str to int
        }


        num_lines++;

        cout << "line " << num_lines << ": " << line << endl;
    }

    //cout << endl;
    //cout << "After PROMOTE_ANNOUNCEMENTS:" << endl;

    //system.printAllQueues();

    return 0;
}

