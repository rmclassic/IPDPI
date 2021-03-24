
class TcpPacket
{
public:
  int SourcePort;
  unsigned short int DestPort;
  unsigned int SequenceNumber;
  unsigned int AckNumber;
  short int DataOffset;
  unsigned short int Flags;
  unsigned short int WindowSize;
  unsigned short int Checksum;
  unsigned short int UrgentPointer;
  unsigned int OptionsSize;
  char* Options = NULL;
  unsigned int PayloadSize;
  char* Payload = NULL;

  static TcpPacket* Parse(char* data, int datasize)
  {
    TcpPacket* packet = new TcpPacket;

    packet->SourcePort = ((unsigned int)data[0] << 8) + (unsigned char)data[1];
    packet->DestPort = (data[2] << 8) + data[3];
    packet->SequenceNumber = (data[4] << 24) + (data[5] << 16) + (data[6] << 8) + (data[7]);
    packet->AckNumber = (data[8] << 24) + (data[9] << 16) + (data[10] << 8) + (data[11]);
    packet->DataOffset = ((data[12] >> 4) & 0x0f) * 4;
    packet->Flags = ((data[13] & 0xf) << 8) + data[14];
    packet->WindowSize = (data[15] << 8) + data[16];
    packet->Checksum = (data[17] << 8) + data[18];
    packet->UrgentPointer = (data[19] << 8) + data[20];
    packet->OptionsSize = packet->DataOffset - 20;
    packet->PayloadSize = datasize - packet->DataOffset;

    if (packet->OptionsSize != 0)
    {
      //std::cout << "Setting options " << packet->DataOffset;
      std::cout.flush();
      packet->Options = new char[packet->OptionsSize];
      memcpy(packet->Options, data + 20, packet->OptionsSize);
      //std::cout << "set options";
      std::cout.flush();
    }

    if (packet->PayloadSize != 0)
    {
    //  std::cout << "Setting payload " << datasize;
      std::cout.flush();
      packet->Payload = new char[packet->PayloadSize];
      memcpy(packet->Payload, data + packet->DataOffset, packet->PayloadSize);
    //  std::cout << "set payload";
      std::cout.flush();
    }
    return packet;
  }
};
