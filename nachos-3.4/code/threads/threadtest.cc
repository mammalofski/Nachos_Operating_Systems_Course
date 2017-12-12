// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"

// testnum is set in main.cc
int testnum = 1;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 6; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}

//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t1 = new Thread("forked thread"); //*creating new thread
    Thread *t2 = new Thread("forked thread"); //*creating new thread
    Thread *t3 = new Thread("forked thread"); //*creating new thread
    Thread *t4 = new Thread("forked thread"); //*creating new thread
    Thread *t5 = new Thread("forked thread"); //*creating new thread


    t1->Fork(SimpleThread, 1, 5); //forking thread 1 with priority 5
    t2->Fork(SimpleThread, 2, 4); //forking thread 2 with priority 4
    t3->Fork(SimpleThread, 3, 3); //forking thread 3 with priority 3
    t4->Fork(SimpleThread, 4, 15); //forking thread 4 with priority 15
    t5->Fork(SimpleThread, 5, 1); //forking thread 5 with priority 1

    //------------------------------------------------------------------
    //******HINT: Our Priority scheduling prioritizes threads which have lower priority value******
    //------------------------------------------------------------------

    //SimpleThread(0);
}

void
ThreadTest2(){
	DEBUG('t', "Entering Threadtest2");

    Thread *t1 = new Thread("forked thread"); //*creating new thread
    Thread *t2 = new Thread("forked thread"); //*creating new thread
    Thread *t3 = new Thread("forked thread"); //*creating new thread
    Thread *t4 = new Thread("forked thread"); //*creating new thread

    t1->Fork(SimpleThread, 1, 234); //forking thread 1 with priority 5
    t2->Fork(SimpleThread, 2, 456); //forking thread 2 with priority 4
    t3->Fork(SimpleThread, 3, 645); //forking thread 3 with priority 3
    t4->Fork(SimpleThread, 4, 567); //forking thread 4 with priority 15

    SimpleThread(0); // main always runs first.

    //------------------------------------------------------------------
    //******HINT: Our Priority scheduling prioritizes threads which have lower priority value******
    //------------------------------------------------------------------
}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:
	//ThreadTest1();
	ThreadTest2();
	break;
    default:
	printf("No test specified.\n");
	break;
    }
}

