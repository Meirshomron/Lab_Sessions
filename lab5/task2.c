#define DONE -1
#define RUNNING 1
#define SUSPENDED 0
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "LineParser.h"
#include "JobControl.h"

#define STDOUT 1
#define STDERR 2
void suspendLast(job* currJob);
/*void handleSignal(int signalNum){
    printf("Ignoring this signal: %s\n",strsignal(signalNum));
}*/

job** job_list_ptr;

void execute(cmdLine *pCmdLine, job* jobToRun){
    char* program = pCmdLine->arguments[0];
    char* args[PATH_MAX-1];
    int i;
    for (i=0; i<pCmdLine->argCount; i++){
        args[i]=pCmdLine->arguments[i];
    }
    
    pid_t pid;
    pid = fork();
    if (pid==-1){
    perror("fork error");
    exit(0x55);
    }
    else if (pid==0){ /* Child */
        signal(SIGQUIT, SIG_DFL); 
        signal(SIGCHLD, SIG_DFL); 
        signal(SIGTSTP, SIG_DFL); 
        
        /* Set child group id -> group id */
        pid_t childPid = getpid();
        setpgid(childPid,childPid);

        int retVal=1;
        retVal=execv(program,args);
        if (retVal==-1){
            printf("Program is: %s",program);
            write(STDERR,"Error on execv\n",strlen("Error on execv\n"));
            exit(0x55);
        }
        
    }
    else{
        /* Set child group id -> group id, and make the parent acknowledge it. */
        /*setpgid(pid,pid);*/ /*ASSAF ! - comment this shit */ 
        jobToRun->pgid = pid; /* Update the parent group id as the childs' */ 

        int status;

/*        printf("%d\n", getpid());*/
        if (jobToRun->cmd[strlen(jobToRun->cmd)-2]!='&')
            runJobInForeground (job_list_ptr,jobToRun,0,defaultAttribute, getpid());                     
    }
}

void handleSignal(int signo)
{
    if (signo == SIGUSR1)
        printf("received SIGUSR1\n");
    else if (signo == SIGKILL)
        printf("received SIGKILL\n");
    else if (signo == SIGSTOP)
        printf("received SIGSTOP\n");
    else if (signo == SIGTSTP){
        printf("received SIGTSTP\n");
        signal(SIGTSTP, handleSignal);
    }
}



int main(int argc, char *argv[]){
	job* job_list = NULL;
	job_list_ptr = &job_list;
	
	char input[2048];
   
    signal(SIGTSTP, handleSignal);
    signal(SIGUSR1, handleSignal);
    signal(SIGCHLD, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    /* Set the process group of the shell to its process id (getpid). */	
    pid_t processId = getpid();
    setpgid(processId, processId);

/*    printf("%d\n", getpid());
*/
    /* Save default terminal attributes to restore them back when a process running in the foreground ends.*/
    defaultAttribute = malloc (sizeof(struct termios));

    if (tcgetattr(STDIN_FILENO, defaultAttribute) != 0) /* Save the terminal attributes so we can restore them later. */
    	error("tcgetatt() error");

    while (1){      	
        cmdLine* box;
        char buff2[PATH_MAX + 1];
        char* cwd2;
        cwd2 = getcwd( buff2, PATH_MAX + 1 );
        printf ("%s> ",cwd2);
    	fgets(input,2048,stdin);    	
    	/*if (input[0]=='q' && input[1]==0) {*/
    	if (strcmp(input,"q\n")==0 || (strncmp(input,"quit",4)==0)){
    		break;
    	}

        else if (strncmp(input,"fg ",3)==0){
            
   
            cmdLine* line;
            int number;
            line=parseCmdLines(input);
            number = atoi(line->arguments[1]);
            job* requiredJob = findJobByIndex(job_list, number);
            /*if (requiredJob!=NULL) printOneJob(requiredJob);*/
            if (requiredJob!=NULL){   
                    runJobInForeground (job_list_ptr,requiredJob,1,defaultAttribute, processId);               
            }
        }
         else if (strncmp(input,"bg ",3)==0){
            cmdLine* line;
            int number;
            line=parseCmdLines(input);
            number = atoi(line->arguments[1]);            
            runJobInBackground (*job_list_ptr,number);
        }
    	else if (strcmp(input,"jobs\n")==0){
    		/*printf("inJobs%s\n");*/
    		printJobs(job_list_ptr);
    	}
    	else {
            job* jobToRun;
            jobToRun = addJob(job_list_ptr, input);
            cmdLine* currCmd;
            currCmd = parseCmdLines(jobToRun->cmd); 
            execute(currCmd, jobToRun);           
        
        } 
	}

	freeJobList(job_list_ptr);
    free(defaultAttribute);

    return 0;
}