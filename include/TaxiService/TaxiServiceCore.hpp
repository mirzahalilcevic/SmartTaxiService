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

    json j;
  	try 
    {
      j = json::parse(buffer.c_str()); 
    } 
    catch(...) 
    { 
      std::cout << "[taxi] json parse error" << std::endl;
      return; 
    }
 
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

  inline bool isFirst(caf::io::connection_handle handle, const std::string& id)
      override
  {
    auto it = std::find_if(requests_.begin(), requests_.end(),
        [id](const RequestEntry& re)
        {
          return re.request.id == id;
        });
    
    if (it != requests_.end())
    {
      for (auto taxi : it->taxis) taxi->stateMachine->process_event(Cancel{id});
      requests_.erase(it);
      return true;
    }
    else
      return false;
  }

  inline void sendRequest(const std::string& msg)
  {
    json j;
    try 
    {
      j = json::parse(msg.c_str()); 
    } 
    catch(...) 
    { 
      std::cout << "[taxi] json parse error" << std::endl;
      return; 
    }
 


    Request request{j["id"], j["location"], j["latitude"], j["longitude"]};
    match(request);
  }

  private:
  using RequestEntry = struct { Request request; std::vector<TaxiType*> taxis; };

  caf::event_based_actor* sender_;
  std::vector<TaxiType> taxis_; 
  std::vector<RequestEntry> requests_;

  void dispatch(TaxiType&, const json&);
  void match(const Request&);
};

void TaxiServiceCore::match(const Request& request)
{
  try
  {
    RequestEntry entry;
    entry.request = request;
    requests_.push_back(entry);

    auto it = std::find_if(requests_.begin(), requests_.end(),
        [request](const RequestEntry& re)
        {
          return re.request.id == request.id; 
        });

    for (auto& taxi : taxis_)
    {
      if (!taxi.isLocationReceived) continue;

   	  double radius = distanceEarth(request.latitude, request.longitude, 
          taxi.data.latitude, taxi.data.longitude);

      if (radius < 1.0)
      {
        it->taxis.push_back(&taxi);
        taxi.stateMachine->process_event(request);
      }
    }
	}
  catch (const std::logic_error e)
  {
    std::cerr << e.what() << std::endl;
	}
}

} // TaxiService