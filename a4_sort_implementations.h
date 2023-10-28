// a4_sort_implementations.h

#pragma once

#include "a4_base.h"
//
// Do NOT add any other #includes to this file!
//

using namespace std;

//
// Put the implementations of all the functions listed in a4_base.h here, as
// well as is_sorted and rand_vec. You can use other helper functions if needed.
//

template <typename T>
bool is_sorted(vector<T> &v)
{
    for (int i = 1; i < v.size(); i++)
    {
        if (v[i-1] > v[i]) { return false; } // Not sorted in ascending order
    }
    return true; // Sorted in ascending order
}


vector<int> rand_vec(int n, int min, int max)
{ 
    int rand_num;
    vector<int> vec;
    for (int i = 0; i < n; i++)
    {
        rand_num = rand() % (max + 1 - min) + min; // Generates random number between given range
        vec.push_back(rand_num);
    }
    return vec;
}


// DELETE
template <typename T>
void print_vec(vector<T> &vec)
{
    for (int i = 0; i < vec.size(); i++)
    {
        cout << vec[i] << " ";
    }
    cout << endl;
}


// Adapted from https://www.geeksforgeeks.org/bubble-sort/
template <typename T>
Sort_stats bubble_sort(vector<T> &v)
{
    ulong num_comps = 0;
    clock_t start = clock();

    for (int i = 0; i < v.size(); i++)
    {
        for (int j = 0; j < v.size() - i - 1; j++)
        {
            num_comps++;
            if (v[j] > v[j+1])
            {
                std::swap(v[j], v[j+1]);
            }
        }
    }
    
    clock_t end = clock();
    double elapsed_cpu_time_sec = double(end - start) / CLOCKS_PER_SEC;
    
    return Sort_stats{"bubble sort",
                      v.size(),
                      num_comps,
                      elapsed_cpu_time_sec};
}

/*
template <typename T>
Sort_stats insertion_sort(vector<T> &v)
{

}


template <typename T>
Sort_stats selection_sort(vector<T> &v)
{

}


template <typename T>
Sort_stats shell_sort(vector<T> &v)
{

}


template <typename T>
Sort_stats merge_sort(vector<T> &v)
{

}


template <typename T>
Sort_stats quick_sort(vector<T> &v)
{

}


template <typename T>
Sort_stats iquick_sort(vector<T> &v)
{

}


template <typename T>
Sort_stats priority_queue_sort(vector<T> &v)
{

}

*/