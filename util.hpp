#pragma once
void Inject(int* dest, char* source, int bytes = 4)
{
  (*dest) = 0;
  for (int i = 0; i < bytes; i++)
  {
    (*dest) += source[i] << ((bytes - i) * 8);
    std::cout << (*dest) << ' ';
  }
}

void PrintPacket(char* data, int size)
{
  for (int i = 0; i < size; i++)
  {
    std::cout << std::hex << (short int)(unsigned char)data[i] << ' ';
  }
  std::cout << '\n';
}
