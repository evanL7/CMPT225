// a1_main.cpp

// This program creates a Wordlist class that stores words without any duplicates. 
// The class is implemented using a doubly-linked list.

// Usage:
// >> make a1_main
// >> ./a1_main < small.txt # where small.txt is a file containing words

#include "Wordlist.h"
#include "test.h"

void read_from_terminal()
{
    Wordlist lst;

    string w;
    while (cin >> w)
    {
        lst.add_word(w);
    } 

    // print the words in sorted order
    vector<string *> index = lst.get_sorted_index();
    
    for (int i = 0; i < index.size(); i++)
    {
        cout << (i + 1) << ". " << *index[i] << endl;
    }
}

void test_Wordlist()
{
    Test("test_Wordlist");

    Wordlist lst;
    
    // assert(!lst.is_frozen());
    // assert(lst.length() == 0);
    // assert(!lst.contains("hello"));

} // test_Wordlist

int main()
{
    read_from_terminal();
}
