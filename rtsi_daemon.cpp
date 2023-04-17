#include <stdio.h> //printf
#include <stdlib.h> //exit
#include <unistd.h> //fork, chdir, sysconf
#include <signal.h> //signal
#include <sys/types.h>
#include <sys/stat.h> //umask
#include <syslog.h> //syslog, openlog, closelog

#include <cs_rtsi/rtsi.h>
#include <cs_rtsi/rtsi_receive_interface.h>

#include "rtsi_pnet/eli_profinet.h"

struct module_joints_t {
    float joint_0_pos;
    float joint_1_pos;
    float joint_2_pos;
    float joint_3_pos;
    float joint_4_pos;
    float joint_5_pos;
    float joint_0_vel;
    float joint_1_vel;
    float joint_2_vel;
    float joint_3_vel;
    float joint_4_vel;
    float joint_5_vel;
    float joint_0_current;
    float joint_1_current;
    float joint_2_current;
    float joint_3_current;
    float joint_4_current;
    float joint_5_current;
    float joint_0_temp;
    float joint_1_temp;
    float joint_2_temp;
    float joint_3_temp;
    float joint_4_temp;
    float joint_5_temp;
    uint8_t joint_0_mode;
    uint8_t joint_1_mode;
    uint8_t joint_2_mode;
    uint8_t joint_3_mode;
    uint8_t joint_4_mode;
    uint8_t joint_5_mode;
    uint16_t Reserved;
};

static void my_daemon()
{
    const char* my_name = "RTSI_Daemon";
    const char* path = "/";

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
    eli_pnet_init();
    const std::string hostip = "192.168.133.130";
    double frequency = 250;
    std::vector<std::string> variables;
    bool verbose = true;

    RTSIReceiveInterface rtsi_receive(hostip, frequency, variables, verbose);

    struct module_joints_t joint;
    std::vector<double> joint_temp;

    while (1)
    {
        syslog(LOG_NOTICE, "RTSI Daemon started");
        
        joint_temp = rtsi_receive.getActualJointPositions();
        joint.joint_0_pos = joint_temp[0];
        joint.joint_1_pos = joint_temp[1];
        joint.joint_2_pos = joint_temp[2];
        joint.joint_3_pos = joint_temp[3];
        joint.joint_4_pos = joint_temp[4];
        joint.joint_5_pos = joint_temp[5];

        joint_temp = rtsi_receive.getActualJointSpeeds();
        joint.joint_0_vel = joint_temp[0];
        joint.joint_1_vel = joint_temp[1];
        joint.joint_2_vel = joint_temp[2];
        joint.joint_3_vel = joint_temp[3];
        joint.joint_4_vel = joint_temp[4];
        joint.joint_5_vel = joint_temp[5];

        joint_temp = rtsi_receive.getActualJointCurrent();
        joint.joint_0_current = joint_temp[0];
        joint.joint_1_current = joint_temp[1];
        joint.joint_2_current = joint_temp[2];
        joint.joint_3_current = joint_temp[3];
        joint.joint_4_current = joint_temp[4];
        joint.joint_5_current = joint_temp[5];

        joint_temp = rtsi_receive.getJointTemperatures();
        joint.joint_0_temp = joint_temp[0];
        joint.joint_1_temp = joint_temp[1];
        joint.joint_2_temp = joint_temp[2];
        joint.joint_3_temp = joint_temp[3];
        joint.joint_4_temp = joint_temp[4];
        joint.joint_5_temp = joint_temp[5];

        std::vector<int> temp = rtsi_receive.getJointMode();
        joint.joint_0_mode = temp[0];
        joint.joint_1_mode = temp[1];
        joint.joint_2_mode = temp[2];
        joint.joint_3_mode = temp[3];
        joint.joint_4_mode = temp[4];
        joint.joint_5_mode = temp[5];

        sleep(10);
        break;
    }
    for(auto i : joint_temp)
    {
        std::string s = std::to_string(i);
        char* ch = new char[s.length() + 1];
        strcpy(ch, s.c_str());           
        syslog(LOG_NOTICE, ch);
    }

    syslog(LOG_NOTICE, "RTSI Daemon terminated");
    closelog();

    return EXIT_SUCCESS;
}