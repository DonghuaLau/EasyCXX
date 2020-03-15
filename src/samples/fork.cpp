#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h> 
#include <unistd.h> 

#define   MAX_COUNT  5
//#define   MAX_COUNT  10

void  ChildProcess(void);
void  ParentProcess(void);

void test2(void)
{
    pid_t  pid;

    pid = fork();
	printf("pid: %d\n", pid);
    if (pid == 0) 
        ChildProcess();
    else 
        ParentProcess();
}

int test_zombie() 
{ 
    // Fork returns process id 
    // in parent process 
    pid_t child_pid = fork(); 
  
    // Parent process  
    if (child_pid > 0) 
        sleep(50); 
  
    // Child process 
    else        
        exit(0); 
  
    return 0; 
} 


void  ChildProcess(void)
{
    int   i;

	pid_t pid = getpid();
	printf("child pid: %d\n", pid);

    for (i = 1; i <= MAX_COUNT; i++)
	{
       printf("   This line is from child, value = %d\n", i);
	   sleep(1);
	}
    printf("   *** Child process is done ***\n");
}

void  ParentProcess(void)
{
    int   i;

	pid_t pid = getpid();
	printf("parent pid: %d\n", pid);

	exit(0); // for test

    for (i = 1; i <= MAX_COUNT; i++)
	{
        printf("This line is from parent, value = %d\n", i);
		sleep(1);
	}
    printf("*** Parent is done ***\n");
}

int main() 
{ 
    //forkexample(); 
	test2();
	//test_zombie();
    return 0; 
} 
