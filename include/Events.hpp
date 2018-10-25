#pragma once

#include <string>

namespace Service {

struct Request
{
  std::string location;
  double latitude;
  double longitude;
};

struct Response
{

};

} // Service
