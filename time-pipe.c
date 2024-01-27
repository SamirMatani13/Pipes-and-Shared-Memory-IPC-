// All the necessary includes listed here
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/time.h>

// Initialized the BUFFER_SIZE to 25 for the message pipe
#define BUFFER_SIZE 25

// Initialized the READ_END of the message pipe to 0 
#define READ_END 0
// Initialized the WRITE_END of the message pipe to 1
#define WRITE_END 1

// Declared the global variables which will contain the times for child, parent, and time between them, respectively
struct timeval childTime;
struct timeval parentTime;
struct timeval timeElapsed;

int main(int argc, char *argv[])
{
	// Declared the process ID
	pid_t pid;
	// Initialized the file descriptors to have a size of 2. 0 for Read and 1 for Write
	int fd[2];


	// If the pipe(fd) equals negative one, then the pipe fails
	if(pipe(fd)==-1){
		fprintf(stderr,"Pipe failed");
		return 1;
	}

	// forks a child process 
	pid = fork();

	// If the pid is less than 0, the fork fails
	if(pid<0){
		fprintf(stderr, "Fork failed");
		return 1;
	}

	// This is the parent block
	if(pid>0){
		// waits for child process to terminate
		wait(NULL);
		// The parent block will close the WRITE_END because it only needs to read
		close(fd[WRITE_END]);
		// The parent blocks reads the value of the child_current variable from the pipe
		read(fd[READ_END], &childTime, sizeof(struct timeval));
		// This gets the time for the parent
		gettimeofday(&parentTime, NULL);
		// The following function performs the subtraction for parent - child which is stored in time elapsed.
		timersub(&parentTime, &childTime, &timeElapsed);
		// Prints the elapsed time seconds followed by microseconds
		printf("Elapsed time: %ld.%ld\n", timeElapsed.tv_sec, timeElapsed.tv_usec);
		// Once the program prints, it closes the READ_END which will then terminate the program
		close(fd[READ_END]);
	}

	// This is the child block
	else{
		// The child closes the READ_END because it only needs to write
		close(fd[READ_END]);
		// This gets the time of day for the child
		gettimeofday(&childTime, NULL);
		// This writes the value of child_current in the pipe for the parent to read
		write(fd[WRITE_END], &childTime, sizeof(struct timeval));
		// After the child is done writing, it closes the WRITE_END
		close(fd[WRITE_END]);
		// This is the exec function which will overwrite the current program with a new one
		execvp(argv[1],&argv[1]);
	}
	
	return 0;
}

		
