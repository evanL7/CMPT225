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


// Helper function for insertion sort as well as iquicksort
template <typename T>
void insertion_sort_void(vector<T> &v, ulong &num_comps)
{
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
}


// Adapted from https://www.geeksforgeeks.org/insertion-sort/
template <typename T>
Sort_stats insertion_sort(vector<T> &v)
{
    ulong num_comps = 0;
    clock_t start = clock();

    insertion_sort_void(v, num_comps);

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


// Helper function for merge sort
template <typename T>
void merge(vector<T> &left, vector<T> &right, vector<T> &v, ulong &num_comps)
{
    int L_sz = left.size();
    int R_sz = right.size();
    int i = 0, j = 0, k = 0;

    while (j < L_sz && k < R_sz)
    {
        num_comps++;
        if (left[j] < right[k])
        {
            v[i] = left[j];
            j++;
        }
        else
        {
            v[i] = right[k];
            k++;
        }
        i++;
    }
    while (j < L_sz)
    {
        v[i] = left[j];
        i++; j++;
    }
    while (k < R_sz)
    {
        v[i] = right[k];
        i++; k++;
    }
}


// Helper function for merge sort
template <typename T>
void merge_sort_void(vector<T> &v, ulong &num_comps)
{
    if (v.size() <= 1) { return; }

    int mid = v.size() / 2;
    vector<T> left;
    vector<T> right;

    // Copies data to left vector and right vector
    for (int i = 0; i < mid; i++)
    {
        left.push_back(v[i]);
    }
    for (int i = 0; i < v.size()-mid; i++)
    {
        right.push_back(v[i+mid]);
    }

    merge_sort_void(left, num_comps);
    merge_sort_void(right, num_comps);
    merge(left, right, v, num_comps);
}


// Adapted from https://www.geeksforgeeks.org/merge-sort/
template <typename T>
Sort_stats merge_sort(vector<T> &v)
{
    ulong num_comps = 0;
    clock_t start = clock();

    merge_sort_void(v, num_comps);

    clock_t end = clock();
    double elapsed_cpu_time_sec = double(end - start) / CLOCKS_PER_SEC;
    
    return Sort_stats{"merge sort", v.size(), num_comps, elapsed_cpu_time_sec};
}


// Helper function for quick sort
template <typename T>
int partition(vector<T> &v, int low, int high, ulong &num_comps)
{
    T pivot = v[high];

    int i = low-1;
    for (int j = low; j <= high; j++)
    {
        num_comps++;
        if (v[j] < pivot)
        {
            i++;
            std::swap(v[i], v[j]);
        }
    }
    num_comps++;
    i++;
    std::swap(v[i], v[high]);

    return i;
}


// Helper function for quick sort
template <typename T>
void quick_sort_void(vector<T> &v, int low, int high, int threshold, ulong &num_comps)
{
    if (low + threshold < high)
    {
        int pivot = partition(v, low, high, num_comps);
        quick_sort_void(v, low, pivot-1, threshold, num_comps); // Recursively sorts elements to the left of the pivot
        quick_sort_void(v, pivot+1, high, threshold, num_comps); // Recursively sorts elements to the right of the pivot
    }
}


// Adapted from https://www.geeksforgeeks.org/quick-sort/
template <typename T>
Sort_stats quick_sort(vector<T> &v)
{
    ulong num_comps = 0;
    clock_t start = clock();

    quick_sort_void(v, 0, v.size()-1, 0, num_comps);

    clock_t end = clock();
    double elapsed_cpu_time_sec = double(end - start) / CLOCKS_PER_SEC;
    
    return Sort_stats{"quick sort", v.size(), num_comps, elapsed_cpu_time_sec};
}


// Adapted from https://www.geeksforgeeks.org/quick-sort/
template <typename T>
Sort_stats iquick_sort(vector<T> &v)
{
    ulong num_comps = 0;
    clock_t start = clock();

    int threshold;
    if (v.size() >= 1000) // If size is greater than 999, quicksort and insertion sort used
    {
        threshold = 25;
        quick_sort_void(v, 0, v.size()-1, threshold, num_comps);
        insertion_sort_void(v, num_comps);
    }
    else // If size is less than 1000, only quicksort used
    {
        threshold = 0;
        quick_sort_void(v, 0, v.size()-1, threshold, num_comps);
    }

    clock_t end = clock();
    double elapsed_cpu_time_sec = double(end - start) / CLOCKS_PER_SEC;
    
    return Sort_stats{"iquick sort", v.size(), num_comps, elapsed_cpu_time_sec};
}


// Helper class for priority queue sort
// Adapted from week 7 lecture notes
template <typename T>
class PriorityQueue_heap
{
    vector<T> v;
    ulong num_comps = 0; // Stores the number of comparisons within the class

public:

    // Returns the number of elements in the priority queue
    int size() const { return v.size(); }


    bool empty() const { return v.size() == 0; }


    ulong comparisons() const { return num_comps; }


    void insert(const T &x)
    {
        v.push_back(x);
        int i = size() - 1;

        while (i > 0 && v[i] < v[(i-1) / 2])
        {
            num_comps++;
            std::swap(v[i], v[(i-1) / 2]);
            i = (i-1) / 2;
        }
    }


    const T &min() const { return v[0]; }


    void remove_min()
    {
        if (empty()) { return; }

        v[0] = v.back();
        v.pop_back();
        int i = 0, j;

        while (2 * i + 1 < size())
        {
            num_comps++;
            j = 2 * i + 1;
            if (j + 1 < size() && v[j + 1] < v[j])
            {
                j++;
            }
            if (v[i] <= v[j])
            {
                break;
            }
            std::swap(v[i], v[j]);
            i = j;
        }
    }
}; // class PriorityQueue_heap


// Adapted from week 7 lecture notes
template <typename T>
Sort_stats priority_queue_sort(vector<T> &v)
{
    clock_t start = clock();

    PriorityQueue_heap<T> pq;

    while (v.size() > 0) // Loop to put all elements into the priority queue
    {
        pq.insert(v.back());
        v.pop_back();
    }
    while (pq.size() > 0)
    {
        v.push_back(pq.min()); // Push min element back to the vector
        pq.remove_min(); // Remove min from the priority queue
    }

    clock_t end = clock();
    double elapsed_cpu_time_sec = double(end - start) / CLOCKS_PER_SEC;
    
    return Sort_stats{"priority queue sort", v.size(), pq.comparisons(), elapsed_cpu_time_sec};
}
