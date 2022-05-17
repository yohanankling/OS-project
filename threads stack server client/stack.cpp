//
// Created by yohanan on 10/05/22.
//

// stack from https://www.codegrepper.com/code-examples/cpp/stack+using+linked+list+in+c%2B%2B
#include "stack.hpp"
#include <mutex>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

int size = 0;
int blocksize = sizeof (struct node);
pthread_mutex_t mutex1;

void* malloc (uint) {
    pthread_mutex_lock(&mutex1);
    void* ptr = sbrk((size+1)*blocksize);
    pthread_mutex_unlock(&mutex1);
    return ptr;
}

void* calloc (uint, uint) {
    pthread_mutex_lock(&mutex1);
    void* ptr = sbrk((size+1)*blocksize);
    pthread_mutex_unlock(&mutex1);
    return ptr;
}

void free (void*) {
    // no need to lock because its part of locked function
    sbrk(size*blocksize);
}

Stack::Stack()
{
    this->top = NULL;
}
void Stack::Push(string &n)
{
    pthread_mutex_lock(&mutex1);
    size++;
    node* temp = new node();
    temp->data = n;
    temp->next = NULL;
    if (top == NULL)
    {
        top = temp;
    }
    else
    {
        temp->next = top;
        top = temp;
    }
    pthread_mutex_unlock(&mutex1);
}

string Stack::Pop()
{
    pthread_mutex_lock(&mutex1);
    if (top == NULL)
    {
        pthread_mutex_unlock(&mutex1);
        return "Error,Stack is Empty!";
    }
    node* temp = top;
    string data = top->data;
    top = top->next;
    size--;
    free(temp);
    pthread_mutex_unlock(&mutex1);
    return data;
}

string Stack::Top()
{
    pthread_mutex_lock(&mutex1);
    if (top == NULL)
    {
        pthread_mutex_unlock(&mutex1);
        return "Error,Stack is Empty!";
    }
    pthread_mutex_unlock(&mutex1);
    return top->data;
}

Stack::~Stack(){
    pthread_mutex_lock(&mutex1);
    while(this->Pop()!= ""){
        this->Pop();
    }
    pthread_mutex_unlock(&mutex1);
}

//int main() {
//    Stack s;
//    printf("\tMENU");
//    printf("\n----------------------------\n");
//    printf("      1. PUSH TEXT FOR EXAMPLE\n");
//    printf("      2. POP\n");
//    printf("      3. TOP\n");
//    printf("----------------------------\n");
//    while (true) {
//        printf("Choose operation : ");
//        string str;
//        string word;
//        getline(cin, str);
//        if (str.rfind("PUSH ", 0) == 0){
//            for (int i = 5; i < str.size(); ++i) {
//                word += str[i];
//            }
//            if (word.size() != 0) {
//                s.Push(word);
//            } else printf("you tried to insert an empty word...");
//        }
//        else if (str.rfind("POP", 0) == 0) {
//            string ans = s.Pop();
//            cout << ans << endl;
//        }
//        else if (str.rfind("TOP", 0) == 0) {
//            string x = s.Top();
//            printf("the top is: %s\n", x.c_str());
//        }
//        else {
//            printf("Invalid operation \n");}
//    }
//    return 0;
//}
