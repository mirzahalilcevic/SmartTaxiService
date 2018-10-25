#include "TaxiService/TaxiServiceCore.hpp"
#include "Tools/haversine.hpp"
#include "Tools/json.hpp"

namespace TaxiService {

using json = nlohmann::json;

void TaxiServiceCore::dispatch(Taxi& taxi, const std::vector<char>& buf)
{
	std::string msg;
  msg.reserve(buf.size());
	std::copy(buf.begin(), buf.end() - 2, msg.begin());
	auto j = json::parse(msg.c_str());
  
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

Taxi& TaxiServiceCore::getNearestTaxi(const Request& request)
{
	auto target = activeTaxis_.begin();
	for (auto it = activeTaxis_.begin() + 1; it != activeTaxis_.end(); ++it)
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

} // TaxiService
