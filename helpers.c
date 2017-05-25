/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>
#include "helpers.h"

bool recursive_search(int value, int values[], int n, int start, int end);

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    int start = 0;
    int end = n;
    return recursive_search(value, values, n, start, end);
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // initialize a counting array of size 65536 to hold all 0's
    int count_array[65536] = {0};
    // iterate through each value in the input array
    for (int i = 0; i < n; i++)
    {
        // increments the appropriate index in the counting array
        count_array[values[i]]++;
        // clears the input array
        values[i] = 0;
    }
    // initialize indexing variable to point to the next spot in the sorted array
    int counter = 0;
    // iterate through each index in the counting array
    for (int j = 0; j < 65536; j++)
    {
        while (count_array[j] > 0)
        {
            // set current index in the sorted array to equal the index of the counting array
            values[counter] = j;
            // decrement the current index in the counting array
            count_array[j]--;
            // move to the next index in the sorted array
            counter++;
        }
    }
    return;
}

bool recursive_search(int value, int values[], int n, int start, int end)
{
    // return false if subarray in invalid
    if (n <= 0)
    {
        return false;
    }
    // if length of subarray is 1, compare the search value with that of the lone element in the subarray
    else if (start == end)
    {
        if (values[n] == value)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        // calculate the midpoint index of the current subarray
        int mid = (start + end) / 2;
        // compare the search value with that at the midpoint of the subarray
        if (values[mid] == value)
        {
            return true;
        }
        // if the search value is smaller than the midpoint
        else if (values[mid] > value)
        {
            // recursively search for the search value in the left half of the array
            return recursive_search(value, values, mid+1, 0, mid);
        }
        // if the search value is larger than the midpoint
        else
        {
            // recursively search for the search value in the right half of the array
            return recursive_search(value, values, mid, mid+1, end);
        }
    }
    return false;
}
