#include "TaxiService/TaxiServiceCore.hpp"

namespace TaxiService {

void TaxiServiceCore::dispatch(TaxiType& taxi, const json& j)
{
  if (j["type"] == "location")
  {
    taxi.data.latitude = j["latitude"];
    taxi.data.longitude = j["longitude"];
    taxi.isLocationReceived = true;
  }
  else if (j["type"] == "response")
  {
    Response response{j["id"], j["accept"], 0.0};
    taxi.stateMachine->process_event(response);
  }
}

void TaxiServiceCore::removeGarbage(std::string& buffer)
{
  for (auto it = buffer.rbegin(); it != buffer.rend(); ++it)
  {
    if (*it == '}')
      break;
    else
      buffer.pop_back();
  }
}

} // TaxiService
