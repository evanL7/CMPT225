// a5_main.cpp

// This program creates a Wordlist class that stores words without any duplicates. 
// The class is implemented using an AVL tree data structure.

// Usage:
// >> make a5_main
// >> ./a5_main < small.txt # small.txt is a file that contains a list of words
// or
// can change the filename in the main function to test with a different file

#include "Wordlist.h"

// NOTE: requires filename to be passed in as a command line argument
// test_read() is a helper function that reads words, one at a time, from cin,
// into a Wordlist and print the stats. You can use it to help test your
// Wordlist class, e.g.:
//
//    > make a5_main
//    g++ -std=c++17 -Wall -Wextra -Werror -Wfatal-errors -Wno-sign-compare -Wnon-virtual-dtor -g   a5_main.cpp   -o a5_main
//   
//    > ./a5_main < tiny_shakespeare.txt
//    Number of different words: 25670
//        Total number of words: 202651
//           Most frequent word: the 5437
//         Number of singletons: 14919 (58%)
//
void test_read()
{
    Wordlist lst;
    string w;
    while (cin >> w)
    {
        lst.add_word(w);
    }

    lst.print_words();
    lst.print_stats();

    if (lst.is_sorted())
    {
        cout << "list sorted. good" << endl;
    }
}

int main()
{
    // test_read();
    Wordlist lst("small.txt");
    lst.print_words();  
    lst.print_stats();                         
}
