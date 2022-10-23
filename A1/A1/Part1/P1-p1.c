#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
	pid_t pid;
	pid_t child_pid;

	pid = fork();

	if (pid == 0) { /* child process */
	  struct timeval tv;
	  struct timezone tz;
	  struct tm *today;
	  
	  child_pid = getpid();

	  while (true) {
	    /*to get time w. r. to 1/1/1970*/
  	    gettimeofday(&tv,&tz);

	    /*converting time to understandable values*/
	    today = localtime(&tv.tv_sec);
	    printf("%d:%0d:%0d.%ld\n", today->tm_hour, today->tm_min, today->tm_sec, tv.tv_usec);   
  	    
            /*printing child PID*/	    
	    printf("Child process id %d\n", child_pid);
	    
	    sleep(1);
	  }

	  return 0;
	}
	else if (pid > 0) { /* parent process */
	  sleep(5);
	 
	  printf("Parent process id %d\n", getpid());
	  kill(pid, SIGKILL);
	  printf("Child <%d> killed.\n", pid);
	  wait(NULL);
	  exit(0);
	  return 0;
	}
}





