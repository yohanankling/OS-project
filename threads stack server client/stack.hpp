//
// Created by yohanan on 10/05/22.
//
#include <cstdio>
#include <string>
#include<iostream>
using namespace std;
struct node
{
    string data;
    node* next;
};

class Stack{
private:
    node* top;
public:
    Stack ();
    ~Stack ();
    void Push(string &n);
    string Pop();
    string Top();
};

