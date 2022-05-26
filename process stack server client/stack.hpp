//
// Created by yohanan on 10/05/22.
//
#include <cstdio>
#include <string>
#include<iostream>
#include<unistd.h>
using namespace std;

struct Node
{
    char data[1024];
    Node* prev;
};

class Stack{
public:
    char* address;
    Node* top;
    int size;
    Stack() = default;
    ~Stack() = default;
    void Push(string &n);
    string Pop();
    string Top() const;
};

