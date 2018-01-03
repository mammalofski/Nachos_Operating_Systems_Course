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

void myRun (int myFunc) {
	//currentThread->RestoreUserState();
	printf("fuck you pid: %d\n", currentThread->getPid());
	machine->WriteRegister(PCReg, myFunc);
	machine->WriteRegister(NextPCReg, myFunc + 4);
	printf("running the freaking child pid: %d\n", currentThread->getPid());
	//machine->WriteRegister(2, currentThread->getPid());
	printf("after writeRegyster\n");
	machine->Run();
}

int childIsAlive (int pid) {
	int flag = 0;
		  for (int i = 0; i < currentThread->childCount; i++) {
		      		if (currentThread->child_pids[i] == pid &&
		      				currentThread->child_status[i] == CHILD_LIVE) {
		      			flag = 1;
		      			printf("a live child has been found\n");
		      		}


		      	}
		 return flag;
}

void
ExceptionHandler(ExceptionType which)
{
    int type = machine->ReadRegister(2);
    int nextPCRegTemp = machine->ReadRegister(NextPCReg) + 4;
    Thread * childThread;

    if ((which == SyscallException) && (type == SC_Halt)) {
	DEBUG('a', "Shutdown, initiated by user program.\n");
   	interrupt->Halt();
    } else if ((which == SyscallException) && (type == SC_Fork)) {
    	printf("entering fork pid: %d ppid: %d\n", currentThread->getPid(), currentThread->getPpid());
    	DEBUG('a', "Fork, initiated by user program.\n");
    	int myFunc = machine->ReadRegister(4);

    	machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));

    	machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));

    	machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);


    	Thread *child = new Thread("forked thread");
    	childThread = child;

    	child->parent = currentThread;

    	// Add the child to the parent's list
    	currentThread->initializeChildStatus(child->getPid());

    	// make room for the new born
    	child->space = currentThread->space;


    	// Change the return address register to zero and save state
    	machine->WriteRegister(2, 0);

    	child->SaveUserState();

    	machine->WriteRegister(2, child->getPid());

    	child->Fork(myRun, myFunc);
    	printf("after fork pid: %d ppid: %d\n", currentThread->getPid(), currentThread->getPpid());

    	//currentThread->Yield();
    	//Join(child->getPid());
    	//printf("after Yield pid: %d ppid: %d\n", currentThread->getPid(), currentThread->getPpid());


    } else if ((which == SyscallException) && (type == SC_Exit)) {
    	int exitStatus = machine->ReadRegister(4);
    	printf("exit status: %d\n", exitStatus);
    	printf("currentThread  pid: %d ppid: %d\n", currentThread->getPid(), currentThread->getPpid());

    	if (currentThread->getPpid() > 0)
    		for (int i = 0; i < currentThread->parent->childCount; i++) {
    			if (currentThread->parent->child_pids[i] == currentThread->getPid())
    				currentThread->parent->child_status[i] = BLOCKED;
    		}

    	printf("ending thread pid: %d\n", currentThread->getPid());

    	currentThread->Finish();

    /*	if (currentThread->parent->getPid() > 1) {
    		printf("test 1\n");
    		currentThread->Finish();
    		for (int i = 0; i < currentThread->parent->childCount; i++) {
    			if (currentThread->parent->child_pids[i] == currentThread->getPid()) {
    				currentThread->parent->child_status[i] = BLOCKED;
    				break;
    			}

    		}
    	}

    	int flag = 0;

    	for (int i = 0; i < currentThread->parent->childCount; i++) {
    		if (currentThread->parent->child_status[i] != BLOCKED)
    			flag = 1;

    	}

    	if (flag == 0)
    		currentThread->parent->Finish();*/




    } else if((which == SyscallException) && (type == SC_Yield)){
        // Advance program counters
          machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
          machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
          machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);

          //Already present in thread.cc
          currentThread->Yield();
  } else if ((which == SyscallException) && (type == SC_Join)) {
	  machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));

	      	machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));

	      	machine->WriteRegister(NextPCReg, machine->ReadRegister(NextPCReg)+4);

	  printf("joining pid: %d", currentThread->getPid());
	  int pid = machine->ReadRegister(4);

	  printf("arg pid: %d\n", pid);



	  while (childIsAlive(pid) == 1) {
		  printf("parent is yielding\n");
		  currentThread->Yield();
	  }
	  printf("parent has no alive child\n");

	  //currentThread->Finish();



  } else {
	printf("Unexpected user mode exception %d %d\n", which, type);
	ASSERT(FALSE);
    }
}


