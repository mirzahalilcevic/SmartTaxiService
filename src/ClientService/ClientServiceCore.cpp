#include "ClientService/ClientServiceCore.hpp"

namespace ClientService {

void ClientServiceCore::removeGarbage(std::string& buffer)
{
  for (auto it = buffer.rbegin(); it != buffer.rend(); ++it)
  {
    if (*it == '}')
      break;
    else
      buffer.pop_back();
  }
}

}