#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdbool.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
	pid_t p_pid; //parent pid 
	pid_t pid; //arbitrary pid
	p_pid = getpid();
	pid_t p_array[5];	
	
	pid = getpid();

	int i;
  	for(i=0; i < 4 ; i++)
    	{
      		if (pid > 0){ //making sure that only the original parent create new processes this way the number of children don't increase exponentially
			pid = fork();
			p_array[i] = pid;
			
		}
    	}

	pid = getpid(); //implementing bc the last iteration of above for loop didn't get pid for the last created child
	if (pid != p_pid) { /* child process */
	  struct timeval tv;
	  struct timezone tz;
	  struct tm *today;
	    

	  while (true) {
	    /*to get time w. r. to 1/1/1970*/
  	    gettimeofday(&tv,&tz);

	    /*converting time to understandable values*/
	    today = localtime(&tv.tv_sec);
	    printf("%d:%0d:%0d.%ld\n", today->tm_hour, today->tm_min, today->tm_sec, tv.tv_usec);   
  	    
            /*printing child PID*/	    
	    printf("Child process id %d\n", pid);
	    
	    sleep(1);
	  }

	  return 0;
	}
	else  { /* parent process */
	  sleep(5);
	  printf("Parent process id %d\n", getpid());
	  
	  for (i=0; i<4; i++){
		  kill(p_array[i], SIGKILL);
	  	  printf("Child <%d> killed.\n", p_array[i]);
		  
	  }
	  
	  wait(NULL);
	  return 0;
	}	


}  
