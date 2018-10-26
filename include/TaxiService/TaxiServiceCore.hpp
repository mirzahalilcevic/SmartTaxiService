#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>
#include <boost/sml.hpp>

#include "ServiceCore.hpp"
#include "Messages.hpp"
#include "Tools/json.hpp"
#include "Tools/Haversine.hpp"
#include "TaxiService/TaxiStateTransitions.hpp"

namespace TaxiService {

using json = nlohmann::json;

class TaxiServiceCore : public ServiceCore
{
  public:
  struct TaxiType
  {
    using TaxiData = struct { double latitude; double longitude; };
    using StateMachine = boost::sml::sm<TaxiStateTransitions>;

    caf::actor worker;
    caf::io::connection_handle handle;
    TaxiData data;
    std::unique_ptr<StateMachine> stateMachine;
	  bool isLocationReceived = false;

    TaxiType(ServiceCore* core, caf::actor w, caf::io::connection_handle h)
        : worker{w}, handle{h}, 
        stateMachine{std::make_unique<StateMachine>(core, handle)} {}
  };
  
  inline void init(caf::event_based_actor* sender)
  {
    sender_ = sender;
  }

  inline void subscribe(caf::actor worker, caf::io::connection_handle handle)
  {
    taxis_.push_back(TaxiType{this, worker, handle});
  }

  inline void unsubscribe(caf::io::connection_handle handle)
  {
    auto it = std::find_if(taxis_.cbegin(), taxis_.cend(), 
        [handle](const TaxiType& taxi)
        { 
          return taxi.handle == handle; 
        });
    
    if (it == taxis_.cend())
      return;
    else
      taxis_.erase(it);
  }

  inline void handle(const caf::io::new_data_msg& msg)
  {
   	std::string buffer;
    buffer.reserve(msg.buf.size());
  	std::copy(msg.buf.begin(), msg.buf.end(), buffer.begin());
  	auto j = json::parse(buffer.c_str());
 
    auto it = std::find_if(taxis_.begin(), taxis_.end(), 
        [msg](const TaxiType& taxi)
        { 
          return taxi.handle == msg.handle; 
        });

    if (it != taxis_.end()) dispatch(*it, j);
  }

  inline void send(caf::actor worker, const std::string& data) override
  {
    sender_->send(worker, data);
  }

  inline caf::actor getWorker(caf::io::connection_handle handle) override
  {
    auto it = std::find_if(taxis_.begin(), taxis_.end(), 
        [handle](const TaxiType& taxi)
        { 
          return taxi.handle == handle; 
        });

    if (it != taxis_.end()) 
      return it->worker;
    else
      throw std::logic_error{"taxi not found"};
  }

  inline caf::actor getTaxiService() override
  {
    // nop
  }

  inline void sendRequest(const std::string& msg)
  {
    auto j = json::parse(msg);
    Request request{j["location"], j["latitude"], j["longitude"]};
    matchAndSend(request);
  }

  private:
  caf::event_based_actor* sender_;
  std::vector<TaxiType> taxis_; 

  void dispatch(TaxiType&, const json&);
  bool matchAndSend(const Request&);
  auto& getNearestTaxi(const Request&);
};

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

bool TaxiServiceCore::matchAndSend(const Request& request)
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