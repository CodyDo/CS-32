//
//  linear.cpp
//  Homework 3
//
//  Created by Cody Do on 2/8/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

/*
#include <iostream>
#include <string>
#include <cmath>
using namespace std;

bool somePredicate(double x)
{
    return x < 0;
}
*/

  // Return true if the somePredicate function returns false for at
  // least one of the array elements; return false otherwise.
bool anyFalse(const double a[], int n)
{
    if (n <= 0) //Checks 0 or negative size
        return false;
    if (!somePredicate(*a)) //if somePredicate returns false at this location, return true
        return true;
    else //if somePredicate returns true at this location, continue recusion
        return anyFalse(a+1, n-1);
}

  // Return the number of elements in the array for which the
  // somePredicate function returns true.
int countTrue(const double a[], int n)
{
    if (n <= 0)
        return 0;
    if (somePredicate(*a))
        return 1 + countTrue(a+1, n-1);
    else
        return 0 + countTrue(a+1, n-1);
}


  // Return the subscript of the first element in the array for which
  // the somePredicate function returns true.  If there is no such
  // element, return -1.
int firstTrue(const double a[], int n)
{
    if (n <= 0)  //If input n is <= 0 or if recusion hits 0, return -1
        return -1;
    
    if (somePredicate(*a)) //If starting position of array is true, return 0
        return 0;
    else {
        if (firstTrue(a+1, n-1) == -1) //If we never hit something true, return -1
            return -1;
        else
            return 1 + firstTrue(a+1, n-1); // If something true, increment by one for each recursion we did --> meant to keep track of position in array
    }
}



  // Return the subscript of the smallest element in the array (i.e.,
  // return the smallest subscript m such that a[m] <= a[k] for all
  // k from 0 to n-1).  If the function is told to examine no
  // elements, return -1.
int positionOfSmallest(const double a[], int n)
{
    if (n <= 0)
        return -1;
    int indexOfRest = positionOfSmallest(a+1, n-1) + 1; //Finds the position of the smallest element in the array excluding the first element
    if (a[0] <= a[indexOfRest]) //If first value is less than or equal to the value in the position of the smallest element in the rest of the array, return 0 (pos of the first element)
        return 0;
    else
        return indexOfRest;
}

  // If all n2 elements of a2 appear in the n1 element array a1, in
  // the same order (though not necessarily consecutively), then
  // return true; otherwise (i.e., if the array a1 does not contain
  // a2 as a not-necessarily-contiguous subsequence), return false.
  // (Of course, if a2 is empty (i.e., n2 is 0), return true.)
  // For example, if a1 is the 7 element array
  //    10 50 40 20 50 40 30
  // then the function should return true if a2 is
  //    50 20 30
  // or
  //    50 40 40
  // and it should return false if a2 is
  //    50 30 20
  // or
  //    10 20 20
bool contains(const double a1[], int n1, const double a2[], int n2)
{
    if (n1 <= 0) //If n1 is empty, nothing to compare to, return false; base case 1
        return false;
    if (n2 <= 0 ) //If n2 is empty, then n1 contains n2; base case 2
        return true;
    
    if(a1[0] != a2[0]) //If starting elements of a1 and a2 aren't the same, then check the starting position of the a2[0] with that of the next element in a1. Keeping a2 constant allows the function to see if the elements in a2 appear in the same order in a1.
        return contains(a1+1, n1-1, a2, n2);
    else //If starting elements of a1 and a2 are the same, then check the next elements of both lists.
        return contains(a1+1, n1-1, a2+1, n2-1);
}

int main() {
    double nums[] = {1, 2, -300, -5, -7, -100, 3000, 101, -69, 81};
    double checkNums[] = {1,-5,-100};
    double checkNums2[] = {0, -5, 100};
    if (contains(nums, 10, checkNums, 3))
        cout << "contains works; nums DOES contains checkNums in the same order" << endl;
    else
        cout << "contains is broken; nums DOESN'T contain checkNums in the same order" << endl;
    if (contains(nums, 10, checkNums2, 3))
        cout << "contains is broken; nums DOES contains checkNums in the same order" << endl;
    else
        cout << "contains works; nums DOESN'T contain checkNums in the same order" << endl;
    
    double nums1[] = {1, 2, 3, -5, -7, -100};
    double nums2[] = {-1, -4, -6, -8 ,-10};
    int a = positionOfSmallest(nums1, 6);
    cout << "positionOfSmallest work! Smallest number is at position: " << a << endl << "Should be: 5" << endl;
    
    int b = firstTrue(nums1, 6);
    cout << "firstTrue works! First negative at position: " << b << endl << "Should be: 3" << endl;
    
    int c = countTrue(nums1, 6);
    cout <<"countTrue works! " << c << " item(s) are negative" << endl << "Should be : 3" << endl;
    
    if (anyFalse(nums1, 3))
        cout << "anyFalse works; At least one is positive" << endl;
    else
        cout << "anyFalse is broken; All are negative" << endl;
    if (anyFalse(nums2, 3))
        cout << "anyFalse is broken; At least one is positive" << endl;
    else
        cout << "anyFalse works; All are negative" << endl;
}
