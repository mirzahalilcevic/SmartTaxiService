#include "ServiceCore.hpp"
#include "Haversine.hpp"

namespace Service {

void ServiceCore::dispatch(Taxi& taxi, const std::vector<char>& buf)
{
}

Taxi& ServiceCore::getNearestTaxi(const Request& request)
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

} // Service
