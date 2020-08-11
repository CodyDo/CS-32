//
//  eval.cpp
//  Homework 2
//
//  Created by Cody Do on 2/1/20.
//  Copyright © 2020 Cody Do. All rights reserved.
//

#include "Map.h"
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;

bool isValidInfix(string infix);
void changeToPostfix(string infix, string &postfix);
bool isInMap(string infix, const Map &values);

int evaluate(string infix, const Map& values, string& postfix, int& result) {
    // Evaluates an integer arithmetic expression
    //   If infix is a syntactically valid infix integer expression whose
    //   only operands are single lower case letters (whether or not they
    //   appear in the values map), then postfix is set to the postfix
    //   form of the expression; otherwise postfix may or may not be
    //   changed, result is unchanged, and the function returns 1.  If
    //   infix is syntactically valid but contains at least one lower
    //   case letter operand that does not appear in the values map, then
    //   result is unchanged and the function returns 2.  If infix is
    //   syntactically valid and all its lower case operand letters
    //   appear in the values map, then if evaluating the expression
    //   (using for each letter in the expression the value in the map
    //   that corresponds to it) attempts to divide by zero, then result
    //   is unchanged and the function returns 3; otherwise, result is
    //   set to the value of the expression and the function returns 0.
    
    if (isValidInfix(infix))
        changeToPostfix(infix, postfix);
    else
        return 1;
    
    if (isInMap(infix, values) == false)
        return 2;
    
    stack<int> intStack;
    for (int k = 0; k < postfix.length(); k++) {
        char ch = postfix[k];
        if (islower(ch)) {
            int val;
            values.get(ch, val);
            intStack.push(val);
        }
        else {
            int v2 = intStack.top();
            intStack.pop();
            if (intStack.empty()) //Secondary check for syntax --> this shouldn't happen if isValidInfix works properly
                return 1;
            int v1 = intStack.top();
            intStack.pop();
            if (ch == '*')
                intStack.push(v1*v2);
            else if (ch == '/') {
                if (v2 == 0)
                    return 3;
                else
                    intStack.push(v1/v2);
            }
            else if (ch == '+')
                intStack.push(v1+v2);
            else if (ch == '-')
                intStack.push(v1-v2);
        }
    }
    result = intStack.top();
    return 0;
}

bool isValidInfix(string infix) {
    if(infix == "") //Checks if infix is empty string
        return false;
    
    int numOpenParen = 0;
    int numCloseParen = 0;
    
    for (int k = 0; k < infix.length(); k++) {
        char ch = infix[k];
        
        if(!islower(ch) && ch != '(' && ch != ')' && ch != '*' && ch != '/' && ch != '+' && ch != '-' && ch != ' ') //Checks if string has only letters and acceptable operators
            return false;
        
        if (ch == '*' || ch == '/' || ch == '+' || ch == '-') { //Meant to check for cases where the string has improper use of operators, (two back to back), ex: "a * b + - c"
            char prevChar = '.';
            if (k >= 1) { //Scans through past items until it hits a non-space character
                int pos = k;
                while (pos != 0) {
                    prevChar = infix[pos-1];
                    if (prevChar == ' ')
                        pos--;
                    else
                        break;
                }
            }
            if (!islower(prevChar) && prevChar != ')')
                return false;
        }
        
        if (islower(ch)) {  //Meant to check for cases where the string has two letters with no operator between them, ex: "a * b c + d"
            char prevChar = '.';
            if (k >= 1) {
                int pos = k;
                while (pos != 0) {
                    prevChar = infix[pos-1];
                    if (prevChar == ' ')
                        pos--;
                    else
                        break;
                }
            }
            if (islower(prevChar))
                return false;
        }
        
        if (ch == '(' ) { //Meant to check for proper usage of the open parenthesis
            numOpenParen++;
            char prevChar = '.';
            if (k >= 1) {
                int pos = k;
                while (pos != 0) {
                    prevChar = infix[pos-1];
                    if (prevChar == ' ')
                        pos--;
                    else
                        break;
                }
            }
            if (prevChar != '*' && prevChar != '/' && prevChar != '+' && prevChar != '-' && prevChar != '(' && prevChar != '.') //Checks to make sure the open parenthesis had an operator before it. Exception for '(' allows for nested parentheses, ex: ((x+5)*3) + 2, and exception for '.' is in the case that the string starts off with ( and thus prevChar won't change
                return false;
            
            //Meant to ensure that the open ( has a closing ) somewhere ahead of it. Combats the case of the string being ")*(" --> # of open/closed parentheses match but isn't valid
            int pos = k;
            bool foundClosing = false;
            while (pos != infix.length()) {
                char ch2 = infix[pos];
                if (ch2 == ')') {
                    foundClosing = true;
                    break;
                }
                else
                    pos++;
            }
            if (foundClosing == false)
                return false;
        }
        
        if (ch == ')') { //Meant to check for proper usage of closing parenthesis. Allows nested parentheses and checks to make sure parenthetical statement doesn't end with an operator
            numCloseParen++;
            char prevChar = '.';
            if (k >= 1) {
                int pos = k;
                while (pos != 0) {
                    prevChar = infix[pos-1];
                    if (prevChar == ' ')
                        pos--;
                    else
                        break;
                }
            }
            if (!islower(prevChar) && prevChar != ')')
                return false;
        }
    }
    if (numOpenParen != numCloseParen) //Parentheses exist in pairs
        return false;
    return true;
}

void changeToPostfix(string infix, string &postfix) {
    stack<char> charStack;
    postfix = "";
    
    for (int k = 0; k < infix.length(); k++) {
        char ch = infix[k];
        if (islower(ch)) //if character is lowercase letter, add it to postfix string
            postfix += ch;
        
        if (ch == '(')
            charStack.push(ch);
        
        if (ch == ')') {
            while (charStack.top() != '(') { //Pop off operators until you hit (
                postfix += charStack.top();
                charStack.pop();
            }
            charStack.pop();
        }
        
        if (ch == '*' || ch == '/') { // * and / have higher precedence than + and - so pop off all operators until empty or until hitting +,-,(
            while (!charStack.empty() && charStack.top() != '(' && charStack.top() != '+' && charStack.top() != '-') {
                postfix += charStack.top();
                charStack.pop();
            }
            charStack.push(ch);
        }
        
        if (ch == '+' || ch == '-') { // + and - have lower precedence than * and / so pop off all operators until empty or hitting (
            while (!charStack.empty() && charStack.top() != '(') {
                postfix += charStack.top();
                charStack.pop();
            }
            charStack.push(ch);
        }
    }
    
    while(!charStack.empty()) {
        postfix += charStack.top();
        charStack.pop();
    }
}

bool isInMap(string infix, const Map &values) {
    for (int k = 0; k < infix.length(); k++) { //Scan through infix and check to see if all the letters have a value in Map
        char ch = infix[k];
        if (islower(ch)) {
            int val;
            if (values.get(ch, val) == false)
                return false;
        }
    }
    return true;
}

/*int main()
{
    char vars[] = { 'a', 'e', 'i', 'o', 'u', 'y', '#' };
    int  vals[] = {  3,  -9,   6,   2,   4,   1  };
    Map m;
    for (int k = 0; vars[k] != '#'; k++)
        m.insert(vars[k], vals[k]);
    string pf;
    int answer;
    assert(evaluate("a+ e", m, pf, answer) == 0  &&
                            pf == "ae+"  &&  answer == -6);
    answer = 999;
    assert(evaluate("", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a i", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("ai", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("()o", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(o+u)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("y(*o)", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a+E", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("(a+(i-o)", m, pf, answer) == 1  &&  answer == 999);
      // unary operators not allowed:
    assert(evaluate("-a", m, pf, answer) == 1  &&  answer == 999);
    assert(evaluate("a*b", m, pf, answer) == 2  &&
                            pf == "ab*"  &&  answer == 999);
    assert(evaluate("y +o *(   a-u)  ", m, pf, answer) == 0  &&
                            pf == "yoau-*+"  &&  answer == -1);
    answer = 999;
    assert(evaluate("o/(y-y)", m, pf, answer) == 3  &&
                            pf == "oyy-/"  &&  answer == 999);
    assert(evaluate(" a  ", m, pf, answer) == 0  &&
                            pf == "a"  &&  answer == 3);
    assert(evaluate("((a))", m, pf, answer) == 0  &&
                            pf == "a"  &&  answer == 3);
    cout << "Passed all tests" << endl;
}*/
