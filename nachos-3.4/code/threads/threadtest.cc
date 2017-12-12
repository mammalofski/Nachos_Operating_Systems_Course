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
#include "time.h"
#include <unistd.h>
#include "time.h"
#include <unistd.h>
#include <sys/time.h>
#include "ctime"
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
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
		sleep(1);
		currentThread->Yield();
    }
}

void
SimpleThread2(int which)
{
    int num;
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
		sleep(2);
		currentThread->Yield();
    }
}

void
SimpleThread3(int which)
{
    int num;
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
		sleep(3);
		currentThread->Yield();
    }
}
void
SimpleThread4(int which)
{
    int num;
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
		sleep(4);
		currentThread->Yield();
    }
}
void
SimpleThread5(int which)
{
    int num;
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
		sleep(5);
		currentThread->Yield();
    }
}
//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

unsigned long getTimeStamp2 () {
    struct timeval tv;
    gettimeofday(&tv,NULL);
    unsigned long time_in_micros = 1000000 * tv.tv_sec + tv.tv_usec;
    return time_in_micros;
}


void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t1 = new Thread("forked thread1");
    Thread *t2 = new Thread("forked thread2");
    Thread *t3 = new Thread("forked thread3");
    Thread *t4 = new Thread("forked thread4");
    Thread *t5 = new Thread("forked thread5");

    //
    printf("%s\n", currentThread->getName());
    t1->Fork(SimpleThread, 1);
    t2->Fork(SimpleThread2, 2);
    t3->Fork(SimpleThread3, 3);
    t4->Fork(SimpleThread4, 4);
    t5->Fork(SimpleThread5, 5);
    SimpleThread(0);
    /*
    Thread *t6 = new Thread("forked thread1");
	Thread *t7 = new Thread("forked thread2");
	Thread *t8 = new Thread("forked thread3");
	//Thread *t9 = new Thread("forked thread4");
	//Thread *t10 = new Thread("forked thread5");
    t6->Fork(SimpleThread, 1);
	t7->Fork(SimpleThread2, 2);
	t8->Fork(SimpleThread3, 3);
	//t9->Fork(SimpleThread, 4);
	//t10->Fork(SimpleThread, 5);
*/

}

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
	//currentThread->set_t1(getTimeStamp2 ());
    switch (testnum) {
    case 1:
	ThreadTest1();
	break;
    default:
	printf("No test specified.\n");
	break;
    }
}

