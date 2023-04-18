#ifndef __ELI_SYS_UTILS_H__
#define __ELI_SYS_UTILS_H__

#include <arpa/inet.h>
#include <linux/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

void sys_utils_getip(const char *net_name, char *ip_buf)
{
  struct ifreq temp;
  struct sockaddr_in *myaddr;
  int fd = 0;
  int ret = -1;
  strcpy(temp.ifr_name, net_name);
  
  if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    return;
  
  ret = ioctl(fd, SIOCGIFADDR, &temp);
  close(fd);
  
  if(ret < 0)
    return;

  myaddr = (struct sockaddr_in *)&(temp.ifr_addr);
  strcpy(ip_buf, inet_ntoa(myaddr->sin_addr));
}

#endif