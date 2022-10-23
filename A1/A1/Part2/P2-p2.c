#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

int main(void)
{
	//creating two pipes for each child: one for reading, one for writing
	int fd1_1[2];
	int fd1_2[2];
	
	int fd2_1[2];
	int fd2_2[2];
	
	// fd[0] -> READ_END
	// fd[1] -> WRITE_END

	/* creating the pipes */
	if (pipe(fd1_1) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}
	
	if (pipe(fd1_2) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}
	
	if (pipe(fd2_1) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}

	if (pipe(fd2_2) == -1) {
		fprintf(stderr,"Pipe failed");
		return 1;
	}

	/*creating two child processes*/
	pid_t p_pid; //parent pid 
	pid_t pid; //arbitrary pid
	p_pid = getpid();
	pid_t p_array[3]; //to store child pid's	
	
	printf("Parent pid : %d\n", p_pid);
	
	pid = getpid(); //this is to initialize the below for loop
	
	printf("pid : %d\n", pid);
		
	int i;
  	for(i=0; i < 2 ; i++)
    	{
      		if (pid > 0){ //making sure that only the original parent creates new processes, this way the number of children don't increase exponentially
			pid = fork();
			sleep(5); //ADDED FOR PROBLEM 2
			p_array[i] = getpid();
			printf("Child pid : %d\n", p_array[i]);
			
		}
    	}
    	
    	pid = getpid();

	if (pid == p_pid) {  /* parent process */
		
		
		close(fd1_1[0]);
		close(fd1_2[1]);
		close(fd2_1[0]);
		close(fd2_2[1]);
		
		int x;
		printf("Input a number: ");
		scanf("%d", &x);
		
		/*writing to the pipe*/
		write(fd1_1[1], &x, sizeof(int));
		sleep(5); //ADDED FOR PROBLEM 2
		write(fd2_1[1], &x, sizeof(int));
		sleep(5); //ADDED FOR PROBLEM 2
		
		wait(NULL); //waiting for the children to finish their processes
		sleep(5); //ADDED FOR PROBLEM 2
		
		int result1;
		int result2;
		
		/*reading from pipes*/
		read(fd1_2[0], &result1, sizeof(int));
		sleep(5); //ADDED FOR PROBLEM 2
		read(fd2_2[0], &result2, sizeof(int));
		sleep(5); //ADDED FOR PROBLEM 2
		
		printf("%d\n", result1 + result2);
		
		close(fd1_1[1]);
		close(fd1_2[0]);
		close(fd2_1[1]);
		close(fd2_2[0]);
		
		kill(0,SIGKILL);
		
		
	}
	else if (pid == p_array[0]) { /* child process 1*/


		close(fd1_1[1]);
		close(fd1_2[0]);
		
		/* reading from the pipe */
		int y;
		read(fd1_1[0], &y, sizeof(int));
		sleep(5); //ADDED FOR PROBLEM 2
		printf("child 1 read %d\n",y);

		int result = y * pid;
		printf("First Child: Input <%d>, Output <%d>*<%d> = <%d>\n", y, y, pid, result);
		
		/*writing to pipe*/
		write(fd1_2[1], &result, sizeof(int));
		sleep(5); //ADDED FOR PROBLEM 2
		printf("Wrote %d\n", result);
		
		close(fd1_1[0]);
		close(fd1_2[1]);
	}
	else if (pid == p_array[1]) { /*child process 2*/
		
		
		close(fd2_1[1]);
		close(fd2_2[0]);
		
		/* reading from the pipe */
		int y;
		read(fd2_1[0], &y, sizeof(int));
		sleep(5); //ADDED FOR PROBLEM 2
		printf("child 1 read %d\n",y);

		int result = pid / y;
		printf("Second Child: Input <%d>, Output <%d>/<%d> = <%d>\n", y, pid, y, result);
		
		write(fd2_2[1], &result, sizeof(int));
		sleep(5); //ADDED FOR PROBLEM 2
		printf("Wrote %d\n", result);
		
		close(fd1_1[0]);
		close(fd1_2[1]);
	}
	
	
	
	
	
	

	return 0;
}
