Operating Systems nachos

Implementing Fork system call:

1. Firstly we had to change the exception.cc in order to add a Fork handler. So when Fork is called an exception raises with the type of SC_Fork and we enter the else if statement. 

2. Our Fork takes a function of type void * and the child which was forked runs the function so then we have to return to the parent's instructions.

3. For implementing the second step we should save the current PCReg into the prevPCReg and the nextPCReg should be saved to current PCReg and the instruction after nextPCReg should be saved to nextPCReg.

4. Afterwards we create the child thread and save the current running thread to child->parent so then we need to add the created child process to parent's children list. Then we save the retutn value of Fork (child's pid ) to the second register ( we need the value in the second register in order to pass it to join ).

5. At the end we call child->Fork(myRun, myFunc) which is thread's Fork and not system call Fork. myRun is a function defined in exception.cc which does the following:
it'll save myFunc's address to the child's current PCReg.
then it'll save the future instruction to be run to nextPCReg.
Finally we call machine->Run so now the child will run the function which was passed to Fork system call.

6. After implementing Fork system call we needed a Join system call which it's job is actually just like wait ( it takes the forked child's pid and yields itself until it's child is done doing it;s job )

7. At the end we handled Exit which finishes the current ( whether parent or child ) if current thread is child it signals the parent that it's done doing it's job and change it's status to BLOCKED and parent continues running the rest of it's instructions.
