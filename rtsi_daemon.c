#include <stdio.h> //printf
#include <stdlib.h> //exit
#include <unistd.h> //fork, chdir, sysconf
#include <signal.h> //signal
#include <sys/types.h>
#include <sys/stat.h> //umask
#include <syslog.h> //syslog, openlog, closelog

static void my_daemon()
{
    char* my_name = "RTSI_Daemon";
    char* path = "/";

    pid_t pid;

    //fork off the parent process
    pid = fork();

    if(pid < 0)
        exit(EXIT_FAILURE);
    
    //let the parent terminate
    if(pid > 0)
        exit(EXIT_SUCCESS);

    //the child process becomes session leader
    if(setsid() < 0)
        exit(EXIT_FAILURE);

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    //fork off for the second time
    pid = fork();

    if(pid < 0)
        exit(EXIT_FAILURE);

    //let the parent terminate
    if(pid > 0)
        exit(EXIT_SUCCESS);

    //set a new file permissions
    umask(0);

    //change working directory to root directory
    chdir(path);

    //close all open file descriptors
    int x;
    for(x = sysconf(_SC_OPEN_MAX); x>=0; x--)
    {
        close(x);
    }

    //open log file
    openlog(my_name, LOG_PID, LOG_DAEMON);
}

int main()
{
    my_daemon();

    while (1)
    {
        syslog(LOG_NOTICE, "RTSI Daemon started");
        sleep(10);
        break;
    }

    syslog(LOG_NOTICE, "RTSI Daemon terminated");
    closelog();

    return EXIT_SUCCESS;
}