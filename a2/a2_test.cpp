// a2_test.cpp

// Usage:
// >> make a2_test
// >> ./a2_test

#include "Stringlist.h"
#include <cassert>

using namespace std;

int main() {
    cout << " ... put your tests here ...\n";

    Stringlist lst1;
    Stringlist lst2;

    //lst1.insert_before(0, "dog");
    //lst1.insert_before(1, " pink  cat ");
    //lst1.insert_back("tree");


    lst1.insert_before(0, "dog");
    lst1.insert_before(1, "cat");
    lst1.insert_back("tree");
    

    lst2.insert_before(0, "yellow");
    lst2.insert_before(1, "green");
    lst2.insert_before(2, "red");
    lst2.insert_before(3, "orange"); 

    //lst1.remove_all();
    cout << lst1; 

    //lst1 = lst2;

    //cout << lst1; 


    cout << lst2; 

    //lst2 = lst1; 
    //lst.remove_at(0);

    //cout << lst;

    //lst.print_stack(); //  

    //lst.undo();  
    //lst.set(2, "cow"); 
    //cout << lst;
    //cout << "Size: " << lst.size() << endl;

    // lst == {"dog", "cat", "tree"}

    //lst.insert_before(3, "hat");
    // lst == {"dog", "cat", "tree", "hat"}
    //cout << lst;

    //lst.undo();
    // lst == {"dog", "cat", "tree"}

    //lst.insert_before(1, "shoe");
    // lst == {"dog", "shoe", "cat", "tree"}

    //lst.undo();
    // lst == {"dog", "cat", "tree"}

    cout << endl;

    lst1.print_stack();
    lst2.print_stack();


    lst2.remove_first("red");

    cout << lst2; 
    lst2.print_stack();

    lst2.undo();
    cout << lst2;
    lst2.print_stack();

    //cout << lst1;

    //cout << lst2;

    //lst1.print_stack(); //     
    
     
    
    //lst2.print_stack(); //    
    
    //lst2.undo(); 

    //cout << lst2;  
 
    //lst1.print_stack(); //            

    return 0;
}
