//
//  tree.cpp
//  Homework 3
//
//  Created by Cody Do on 2/8/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

#include <iostream>
using namespace std;

// Return the number of ways that all n2 elements of a2 appear
  // in the n1 element array a1 in the same order (though not
  // necessarily consecutively).  The empty sequence appears in a
  // sequence of length n1 in 1 way, even if n1 is 0.
  // For example, if a1 is the 7 element array
  //    10 50 40 20 50 40 30
  // then for this value of a2     the function must return
  //    10 20 40            1
  //    10 40 30            2
  //    20 10 40            0
  //    50 40 30            3
int countContains(const double a1[], int n1, const double a2[], int n2)
{ //Based off of linear.cpp's contains function
    if (n1 <= 0) //If n1 is empty, nothing to compare to, return false; base case 1. Returning 0 means that the check has failed and no same order sequence was found.
        return 0;
    
    if (n2 <= 0 ) //If n2 is empty, then n1 contains n2; base case 2.
        return 1;
    
    if (n1 == 1 && n2 == 1 && a1[0] == a2[0]) //If the size of both arrays is one and the elements of both arrays are the same, then return 1. Returning 1 means that the check was successful and one same order sequence was found. This check is what allows the recursion to successfully add a value.
        return 1;
    
    
    if(a1[0] != a2[0])
        //If starting elements of a1 and a2 aren't the same, then check the starting position of the a2[0] with that of the next element in a1. Keeping a2 constant allows the function to see if the elements in a2 appear in the same order in a1.
        return countContains(a1+1, n1-1, a2, n2);
    else
        //If starting elements of a1 and a2 are the same, then check the next elements of both lists. Furthermore, add the total possibilities of the same order repeating in a later portion of the array.
        return countContains(a1+1, n1-1, a2+1, n2-1) + countContains(a1+1, n1-1, a2, n2);
}

  // Exchange two doubles
void exchange(double& x, double& y)
{
    double t = x;
    x = y;
    y = t;
}

  // Rearrange the elements of the array so that all the elements
  // whose value is > separator come before all the other elements,
  // and all the elements whose value is < separator come after all
  // the other elements.  Upon return, firstNotGreater is set to the
  // index of the first element in the rearranged array that is
  // <= separator, or n if there is no such element, and firstLess is
  // set to the index of the first element that is < separator, or n
  // if there is no such element.
  // In other words, upon return from the function, the array is a
  // permutation of its original value such that
  //   * for 0 <= i < firstNotGreater, a[i] > separator
  //   * for firstNotGreater <= i < firstLess, a[i] == separator
  //   * for firstLess <= i < n, a[i] < separator
  // All the elements > separator end up in no particular order.
  // All the elements < separator end up in no particular order.
void separate(double a[], int n, double separator,
                    int& firstNotGreater, int& firstLess)
{
    if (n < 0)
    n = 0;

      // It will always be the case that just before evaluating the loop
      // condition:
      //  firstNotGreater <= firstUnknown and firstUnknown <= firstLess
      //  Every element earlier than position firstNotGreater is > separator
      //  Every element from position firstNotGreater to firstUnknown-1 is
      //    == separator
      //  Every element from firstUnknown to firstLess-1 is not known yet
      //  Every element at position firstLess or later is < separator

    firstNotGreater = 0;
    firstLess = n;
    int firstUnknown = 0;
    while (firstUnknown < firstLess)
    {
    if (a[firstUnknown] < separator)
    {
        firstLess--;
        exchange(a[firstUnknown], a[firstLess]);
    }
    else
    {
        if (a[firstUnknown] > separator)
        {
        exchange(a[firstNotGreater], a[firstUnknown]);
        firstNotGreater++;
        }
        firstUnknown++;
    }
    }
}

  // Rearrange the elements of the array so that
  // a[0] >= a[1] >= a[2] >= ... >= a[n-2] >= a[n-1]
  // If n <= 1, do nothing.
void order(double a[], int n)
{
    if (n <= 1)
        return;
    int middleVal = a[n/2]; //Pulls the middle value of the array
    int firstNotGreater = 0;
    int firstLess = 0;
    
    separate(a, n, middleVal, firstNotGreater, firstLess); //Splits the array with respect to middleVal. firstNotGreater is set equal to the position of the separater while firstLess is set equal to the position right after the separater.
    
    order(a, firstNotGreater); //Organizes the first half up to the separater value inclusive
    order(a+firstLess, n-firstLess); //Organizes the second half after the separater value
}

/*int main() {
    //For example, if a1 is the 7 element array
    //    10 50 40 20 50 40 30
    // then for this value of a2     the function must return
    //    10 20 40            1
    //    10 40 30            2
    //    20 10 40            0
    //    50 40 30            3
    double nums[] = {10, 50, 40, 20, 50, 40 ,30};
    double check1[] = {10, 20 ,40};
    double check2[] = {10, 40, 30};
    double check3[] = {20, 10, 40};
    double check4[] = {50, 40, 30};
    int a = countContains(nums, 7, check1, 3);
    int b = countContains(nums, 7, check2, 3);
    int c = countContains(nums, 7, check3, 3);
    int d = countContains(nums, 7, check4, 3);
    cout << a << endl << b << endl << c << endl << d << endl << "Should be 1,2,0,3" << endl;
    
    double vals[] = {-10, 30, -29, 18, 44, 5, -14};
    cout << "vals array before order called: " << endl;
    for (int k = 0; k < 7; k++)
        cout << vals[k] << endl;
    order(vals, 7);
    cout << "vals array after order called: " << endl;
    for (int k = 0; k < 7; k++)
        cout << vals[k] << endl;
}*/
