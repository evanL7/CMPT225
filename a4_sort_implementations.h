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

    int i, j;
    for (i = 0; i < v.size(); i++)
    {
        for (j = 0; j < v.size() - i - 1; j++)
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
    
    return Sort_stats{"bubble sort", v.size(), num_comps, elapsed_cpu_time_sec};
}


// Adapted from https://www.geeksforgeeks.org/insertion-sort/
template <typename T>
Sort_stats insertion_sort(vector<T> &v)
{
    ulong num_comps = 0;
    clock_t start = clock();

    int i, j;
    for (i = 1; i < v.size(); i++)
    {
        j = i;
        num_comps++;
        while (j > 0 && v[j-1] > v[j])
        {
            num_comps++;
            std::swap(v[j-1], v[j]);
            j--;
        }
    }

    clock_t end = clock();
    double elapsed_cpu_time_sec = double(end - start) / CLOCKS_PER_SEC;
    
    return Sort_stats{"insertion sort", v.size(), num_comps, elapsed_cpu_time_sec};
}


// Adapted from https://www.geeksforgeeks.org/selection-sort/
template <typename T>
Sort_stats selection_sort(vector<T> &v)
{
    ulong num_comps = 0;
    clock_t start = clock();

    int i, j, min;
    for (i = 0; i < v.size(); i++)
    {
        min = i;
        for (j = i+1; j < v.size(); j++)
        {
            num_comps++;
            if (v[min] > v[j]) { min = j; } // Update new min value
        }
        std::swap(v[i], v[min]);
    }

    clock_t end = clock();
    double elapsed_cpu_time_sec = double(end - start) / CLOCKS_PER_SEC;
    
    return Sort_stats{"selection sort", v.size(), num_comps, elapsed_cpu_time_sec};
}


// Adapted from https://www.geeksforgeeks.org/shellsort/
template <typename T>
Sort_stats shell_sort(vector<T> &v)
{
    ulong num_comps = 0;
    clock_t start = clock();

    int gap, i, j;
    for (gap = v.size()/2; gap > 0; gap /= 2)
    {
        for (i = gap; i < v.size(); i++)
        {
            T temp = v[i];
            num_comps++;
            for (j = i; j >= gap && v[j-gap] > temp; j -= gap)
            {
                num_comps++;
                v[j] = v[j-gap];
            }
            v[j] = temp;
        }        
    }

    clock_t end = clock();
    double elapsed_cpu_time_sec = double(end - start) / CLOCKS_PER_SEC;
    
    return Sort_stats{"shell sort", v.size(), num_comps, elapsed_cpu_time_sec};
}

/*
template <typename T>
Sort_stats merge_sort(vector<T> &v)
{
    ulong num_comps = 0;
    clock_t start = clock();

    clock_t end = clock();
    double elapsed_cpu_time_sec = double(end - start) / CLOCKS_PER_SEC;
    
    return Sort_stats{"merge sort", v.size(), num_comps, elapsed_cpu_time_sec};
}


template <typename T>
Sort_stats quick_sort(vector<T> &v)
{
    ulong num_comps = 0;
    clock_t start = clock();

    clock_t end = clock();
    double elapsed_cpu_time_sec = double(end - start) / CLOCKS_PER_SEC;
    
    return Sort_stats{"quick sort", v.size(), num_comps, elapsed_cpu_time_sec};
}


template <typename T>
Sort_stats iquick_sort(vector<T> &v)
{
    ulong num_comps = 0;
    clock_t start = clock();

    clock_t end = clock();
    double elapsed_cpu_time_sec = double(end - start) / CLOCKS_PER_SEC;
    
    return Sort_stats{"iquick sort", v.size(), num_comps, elapsed_cpu_time_sec};
}


template <typename T>
Sort_stats priority_queue_sort(vector<T> &v)
{
    ulong num_comps = 0;
    clock_t start = clock();

    clock_t end = clock();
    double elapsed_cpu_time_sec = double(end - start) / CLOCKS_PER_SEC;
    
    return Sort_stats{"priority queue sort", v.size(), num_comps, elapsed_cpu_time_sec};
}

*/