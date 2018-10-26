#include "TaxiService/TaxiServiceCore.hpp"
#include "Tools/haversine.hpp"

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
    Response response{j["response"]};
    taxi.stateMachine->process_event(response);
  }
}

auto& TaxiServiceCore::getNearestTaxi(const Request& request)
{
	auto target = taxis_.begin();
	for (auto it = taxis_.begin() + 1; it != taxis_.end(); ++it)
	{
		if (!(it->isLocationReceived)) continue;

		if (distanceEarth(request.latitude, request.longitude, it->data.latitude,
				it->data.longitude) < distanceEarth(request.latitude, request.longitude,
				target->data.latitude, target->data.longitude)) 
			target = it;
	}	
	if (!(target->isLocationReceived))
		throw std::logic_error{"no taxi available"};
	else
		return *target;
}

bool TaxiServiceCore::sendRequest(const Request& request)
{
  try
  {
   	auto& taxi = getNearestTaxi(request);
   	taxi.stateMachine->process_event(request);
		return true;
	}
  catch (const std::logic_error e)
  {
    std::cerr << e.what() << std::endl;
		return false;
	}
}

} // TaxiService
