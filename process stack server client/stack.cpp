//
// Created by yohanan on 10/05/22.
//
#include "stack.hpp"
#include <fcntl.h>

int blocksize = sizeof (struct Node);
struct flock locker;

void* malloc (uint, Stack s) {
    void* ptr = sbrk((uint)sizeof (blocksize)*s.size);
    return ptr;
}

void free (void*, Stack s) {
    sbrk((uint)sizeof(blocksize) * s.size-1);
}

void Stack::Push(string &str)
{
    locker.l_type = F_WRLCK;
    // add one more Node to memory address
    this->address += sizeof(blocksize);
    // set this Node to stack's memory address
    Node *temp = (Node*)this->address;
    for (int i = 0; i < str.size(); ++i) {
        temp->data[i] = str[i];
    }
    if (this->top == nullptr)
    {
        this->top = temp;
    }
    else
    {
        temp->prev = this->top;
        this->top = temp;
    }
    size++;
    locker.l_type = F_UNLCK;
}

string Stack::Pop()
{
    locker.l_type = F_WRLCK;
    if (this->size == 0)
    {
        return "Error,Stack is Empty!";
    }
    string data = top->data;
    this->address += -sizeof(blocksize);
    this->top = top->prev;
    this->size--;
    locker.l_type = F_UNLCK;
    return data;
}

string Stack::Top() const
{
    locker.l_type = F_WRLCK;
    if (this->size == 0)
    {
            locker.l_type = F_UNLCK;
        return "Error,Stack is Empty!";
    }
        locker.l_type = F_UNLCK;
    return this->top->data;
}


