#pragma once

#include <string>

namespace TaxiService {

struct Request
{
  std::string location;
  double latitude;
  double longitude;
};

struct Response
{
  bool accept;
};

} // TaxiService
