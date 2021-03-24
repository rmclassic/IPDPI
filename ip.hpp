#include "iostream"
#include "util.hpp"
#include <cstring>
enum class IPServiceType
{

};

enum class IPProtocol
{

};

class IPAddress
{
public:
  char blocks[4];
  static IPAddress Parse(char* ip)
  {
    IPAddress addr;
    memcpy(&addr.blocks, ip, 4);
    return addr;
  }

  std::string Stringify()
  {
    std::string ip = "";
    for (int i = 0; i < 4; i++)
      ip += std::to_string((unsigned char)blocks[i]) + ".";

    ip.erase(ip.end() - 1);


    return ip;
  }
};

class IPPacket
{
public:
  int Version;
  int IHL;
  int TypeOfService;
  int TotalLength;
  int Identification;
  int Flags;
  short int FragmentOffset;
  unsigned char TimeToLive;
  int Protocol;
  int HeaderChecksum;
  IPAddress SourceIP;
  IPAddress DestinationIP;
  char* Extra = NULL;
  int ExtraLength;
  char* Payload = NULL;
  int PayloadLength;

  static IPPacket* Parse(char* data, int datalen)
  {
    IPPacket* packet = new IPPacket;

    packet->Version = data[0] >> 4;
    if (packet->Version != 4)
    {
      std::cout << "Skipping packet: Not an IPV4 packet\n";
      return NULL;
    }

    packet->IHL = (data[0] & 0x0f);
    packet->TypeOfService = data[1];
    packet->TotalLength = ((unsigned char)data[2] << 8) + (unsigned char)data[3];
    packet->Identification = (data[4] << 8) + data[5];
    packet->Flags = data[6] >> 5;
    packet->FragmentOffset = ((data[6] & 0x1f) << 8) + data[7];
    packet->TimeToLive = data[8];
    packet->Protocol = data[9];
    packet->HeaderChecksum = (data[10] << 8) + data[11];
    packet->SourceIP = IPAddress::Parse(data + 12);
    packet->DestinationIP = IPAddress::Parse(data + 16);
    //////////////////
    //OPTIONS HERE
    //////////////////
    packet->PayloadLength = packet->TotalLength - (packet->IHL * 4);
    //std::cout << "payload len is: " << packet->PayloadLength << "IHL: "<< packet->IHL << '\n';
    //std::cout << "Datalen: " << (packet->IHL * 4) << " plen " << packet->TotalLength << '\n';
    std::cout.flush();

    if (packet->PayloadLength != 0)
    {
        packet->Payload = new char[packet->PayloadLength];
        memcpy(packet->Payload, data + (packet->IHL * 4), packet->PayloadLength);
      //  PrintPacket(packet->Payload, packet->PayloadLength);
    }

    return packet;
  }

  ~IPPacket()
  {
    if (Extra != NULL)
      delete[] Extra;
    if (Payload != NULL)
      delete[] Payload;
  }
};
