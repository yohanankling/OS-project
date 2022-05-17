#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <iostream>
using namespace std;
int one = 1;
int zero = 0;

// signal that handle child
void sigchld(int signum){
    cout << "im curious what will happened if i will divide by zero..." << endl;
    fflush(stdout);
    int trouble = one/zero;
}

// signal that handle divide by zero
void sigfpe(int signum){
    cout << "oh! now im here! , maybe its not a good idea to divide by zero huh? lets keep it in an array..." << endl;
    fflush(stdout);
    string message [1];
    message[one] =  "thinking out the box?";
}

// signal that handle segmentation failure like non-existing place in array
void sigsegv(int signum){
    cout << "?! why im here! maybe max slot on an array is just n-1 ?! maybe i should call who summon me.." << endl;
    fflush(stdout);
    raise(SIGUSR1);
}

// signal that handle "child climbing"
void sigusr1(int signum){
    cout << "hey hey! i have my own stuff to handle! and beside, im working only with assembly, wanna see?" << endl;
    fflush(stdout);
    asm("ud2");
}

// signal that handle assembly error
void sigill(int signum){
    cout << "someone can hear me?? >> ohh you are still here?! admin please kill that pid!!" << endl;
    fflush(stdout);
    exit(1);
}

int main()
{
    // make a funcion to handle every signal that mention below
    signal (SIGCHLD, sigchld);
    signal (SIGFPE, sigfpe);
    signal (SIGSEGV, sigsegv);
    signal (SIGUSR1, sigusr1);
    signal (SIGILL, sigill);

    // to reach the first signal of sigchld
   if (!(fork())){
       exit(1);
   }
   // to give the child processes to run
   wait(reinterpret_cast<int *>(5));

    return 0;
}