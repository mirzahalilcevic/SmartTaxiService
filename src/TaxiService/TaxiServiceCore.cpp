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
    Response response{j["id"], j["accept"]};
    taxi.stateMachine->process_event(response);
  }
}

} // TaxiService
