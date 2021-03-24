#include <iostream>
#include <sys/socket.h>
#include <netinet/ip.h>
#include "ip.hpp"
#include "tcp.hpp"
void SetupServerSocket();

int main()
{
  SetupServerSocket();
}

void SetupServerSocket()
{
  int serversock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
  if (serversock == -1)
    std::cout << "Error: " << serversock << '\n';

  char* buff = new char[65535];
  while (true)
  {
    int buffsize = recv(serversock, buff, 65535, 0);

    IPPacket* ip = IPPacket::Parse(buff, buffsize);
    TcpPacket* tcp = TcpPacket::Parse(ip->Payload, ip->PayloadLength);
    std::cout << tcp->PayloadSize << " bytes from " << ip->SourceIP.Stringify()
              << ":" << tcp->SourcePort << " to " << ip->DestinationIP.Stringify()
              << ":" << tcp->DestPort << '\n';
  }

}
