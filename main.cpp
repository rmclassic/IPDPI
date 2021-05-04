#include <iostream>
#include <sys/socket.h>
#include <netinet/ip.h>
#include "ip.hpp"
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include <linux/if.h>
#include <linux/if_tun.h>
#include <fcntl.h>
#include <cstring>
#include <sys/ioctl.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "tcp.hpp"
void SetupServerSocket();
int FindString(const char* instr, const char* str, int instrsize, int strsize);
int ReadBuffer(char*, int, int);
int InitializeTun();

int main()
{
  SetupServerSocket();
}

void SetupServerSocket()
{
  int tun_fd = InitializeTun();
  std::cout << "Initialized tun1\n";
  std::cout.flush();
  char* buff = new char[65535];
  while (true)
  {
    int buffsize = ReadBuffer(buff, 65535, tun_fd);
    PrintPacket(buff, buffsize);

    if (buffsize < 14)
      continue;
    if ((int)buff[4] >> 4 != 0x4)
    {
      std::cout << "Not ipv4 0x" << (int)buff[4] << '\n';
      continue;
    }
    IPPacket* ip = IPPacket::Parse(buff + 4, buffsize);
    if (ip->Protocol != 0x6)
    {
      std::cout << "Not a TCP packet 0x" << ip->Protocol << '\n';
      continue;

    }


    std::cout << "IP Payload:\n";
    PrintPacket(ip->Payload, ip->PayloadLength);
    TcpPacket* tcp = TcpPacket::Parse(ip->Payload, ip->PayloadLength);
    std::cout << tcp->PayloadSize << " bytes from " << ip->SourceIP.Stringify()
              << ":" << tcp->SourcePort << " to " << ip->DestinationIP.Stringify()
              << ":" << tcp->DestPort << '\n';

    // if (-1 != FindString(tcp->Payload, "GET", tcp->PayloadSize, 3))
    //   std::cout << tcp->Payload << '\n';

  }
}

int FindString(const char* instr, const char* str, int instrsize, int strsize)
{
  for (int i = 0; i < instrsize; i++)
  {
    int counter = 0;
    for (counter; instr[i] == str[counter] && counter < strsize; counter++)
    {
      std::cout << "Matched " << instr[i] << '\n';
      std:: cout << instr << '\n';
      i++;
    }
    if (counter == strsize)
      return i;
  }
  return -1;
}


int tun_alloc(char *dev)
{
   struct ifreq ifr;
   int fd, err;

   if( (fd = open("/dev/net/tun", O_RDWR)) < 0 )
   {
     printf("Error openning tun fd");
     return -1;
   }

   memset(&ifr, 0, sizeof(ifr));

   /* Flags: IFF_TUN   - TUN device (no Ethernet headers)
    *        IFF_TAP   - TAP device
    *
    *        IFF_NO_PI - Do not provide packet information
    */
   ifr.ifr_flags = IFF_TUN;
   if( *dev )
   {
     printf("Setting name to %s", dev);
     strncpy(ifr.ifr_name, dev, IFNAMSIZ);

   }

   if( (err = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0 ){
    close(fd);
    printf("Error in ioctl %d\n", errno);
    return err;
   }
   strcpy(dev, ifr.ifr_name);
   return fd;
}


int InitializeTun()
{
    char wow[15] = {"tun1"};
  return tun_alloc(wow);
}



int ReadBuffer(char* buf, int maxbuf, int tun_fd)
{
  return read(tun_fd, buf, maxbuf);
}
