// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	are in machine.h.
//----------------------------------------------------------------------

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    int nextPCRegTemp = machine->ReadRegister(NextPCReg) + 4;

    if ((which == SyscallException) && (type == SC_Halt)) {
	DEBUG('a', "Shutdown, initiated by user program.\n");
   	interrupt->Halt();
    } else if ((which == SyscallException) && (type == SC_Fork)) {
    	printf("entering fork\n");
    	DEBUG('a', "Fork, initiated by user program.\n");
    	int myFunc = machine->ReadRegister(4);
    	printf("myFunc: %d\n",  myFunc);

    	// now make the prevPCReg the current one
    	// copy the address of myFunc to PCReg
    	// and save the next one
    	machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
    	printf("test 1\n");
    	machine->WriteRegister(PCReg, (int) myFunc);
    	printf("test 2\n");
    	//machine->Run();
    	printf("test 3\n");
    	// machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg));
    	printf("test 4\n");

    	// then create the new thread
    	Thread *child = new Thread("forked thread");
    	printf("child pid 0: %d\n", child->getPid());
    	//machine->Run();
    	printf("test 4\n");

    	// Set the parent of the child process
    	child->parent = currentThread;
    	printf("test 5\n");

    	// Add the child to the parent's list
    	currentThread->initializeChildStatus(child->getPid());


    	// make room for the new born
    	child->space = currentThread->space;
    	printf("test 6\n");
    	//child->space = new AddrSpace(currentThread->space->getNumPages(), currentThread->space->getStartPhysPage());

    	// Change the return address register to zero and save state
    	machine->WriteRegister(2, 0);
    	printf("child pid 1: %d\n", child->getPid());
    	child->SaveUserState();

    	// Setting the return value of the parent thread
    	machine->WriteRegister(2, child->getPid());
    	printf("child pid 2: %d\n", child->getPid());

    	// The child is now ready to run
    	IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    	scheduler->ReadyToRun(child);
    	(void) interrupt->SetLevel(oldLevel); // re-enable interrupts

    	machine->Run();



    } else if ((which == SyscallException) && (type == SC_Exit)) {
    	int exitNum = machine->ReadRegister(4);
    	printf("exitNum: %d\n");
    	printf("exiting thread %d \n", currentThread->getPid());

    	machine->WriteRegister(NextPCReg, nextPCRegTemp);






    	interrupt->Halt();


    } else {
	printf("Unexpected user mode exception %d %d\n", which, type);
	ASSERT(FALSE);
    }
}
