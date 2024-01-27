// Below are all the necessary libraries for the program
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/shm.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/wait.h>

// These are the global variables which store the times for the child, parent and time elapsed, respectively
struct timeval childTime;
struct timeval parentTime;
struct timeval timeElapsed;

int main(int argc, char *argv[])
{
	// Initializes the process
	pid_t pid;
	// Initializes the size of the shared memory segment
	const int SIZE = 4096;
	// Initializes the name of the shared memory segment
	const char *name = "Fork";

	// Declares the shared memory segment
	int shm_fd;
	// Declares the pointer for the shared memory segment
	void *ptr;
	// Opens the shared memory segment with the name and makes it available for reading and writing
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	// Sets the size of the shared memory segment to the size we declared earlier
	ftruncate(shm_fd, SIZE);
	// Maps the pointer of the shared memory space to the virtual address space
	ptr = mmap(0,SIZE, PROT_READ|PROT_WRITE,MAP_SHARED, shm_fd, 0);
	// If the pointer equals MAP_FAILED, then the map fails and we exit the program
	if(ptr==MAP_FAILED){
		printf("Map failed\n");
		return -1;
	}

	// forks a child process
	pid = fork();
	
	// The for will fail if the pid is less than 0
	if(pid<0){
		printf("Fork Failed");
		return 1;
	}
	
	// This is the parent block
	else if(pid>0){
		// Waits for the child process to terminate
		wait(NULL);
		// Copies the child_current time from the pointer
		memcpy(&childTime, ptr, sizeof(struct timeval));
		// Gets the time for the parent
		gettimeofday(&parentTime, NULL);
		// Performs the subtraction for parent - child and stores it in time_elapsed
		timersub(&parentTime, &childTime, &timeElapsed);
		// Prints the elapsed time with the seconds and microseconds
		printf("Elapsed time: %ld.%ld\n", timeElapsed.tv_sec, timeElapsed.tv_usec);
		// After the time is printed, we unlink from the shared memory for the program to terminate
		shm_unlink(name);
	}
	// This is the child block
	else{
		// This gets the time of day for the child
		gettimeofday(&childTime, NULL);
		// This copies the time of day for the child to the pointer for the parent to access
		memcpy(ptr, &childTime, sizeof(struct timeval));
		// This is the exec function which overwrites the current program with a new program 
		execvp(argv[1], &argv[1]);
	}

	return 0;
}
