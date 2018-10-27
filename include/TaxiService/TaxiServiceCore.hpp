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
   	// std::string buffer;
    // buffer.reserve(msg.buf.size());
  	// std::copy(msg.buf.begin(), msg.buf.end(), buffer.begin());

    // removeGarbage(buffer);

    json j;
  	try 
    {
      j = json::parse(msg.buf); 
    } 
    catch(const json::exception& e) 
    { 
      std::cout << e.what() << std::endl;
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

  inline bool isFirst(caf::io::connection_handle handle, 
      Response response) override
  {
    auto it = std::find_if(requests_.begin(), requests_.end(),
        [response](const RequestEntry& re)
        {
          return re.request.id == response.id;
        });
    
    if (it != requests_.end())
    {
      // response.time = calculateTime(it->request, handle);

      auto taxiIt = std::find_if(taxis_.begin(), taxis_.end(),
          [handle](const TaxiType& taxi)
          {
            return handle != taxi.handle; 
          });

      response.latitude = taxiIt->data.latitude;
      response.longitude = taxiIt->data.longitude;

      sender_->send(it->clientService, response);

      std::for_each(it->taxis.begin(), it->taxis.end(),
          [=](TaxiType* taxi)
          {
            if (handle != taxi->handle)
              taxi->stateMachine->process_event(Cancel{response.id});
          });
      
      requests_.erase(it);
      return true;
    }
    else
      return false;
  }

  inline void sendRequest(const std::string& msg, caf::actor clientService)
  {
    json j;
    try 
    {
      j = json::parse(msg.c_str()); 
    } 
    catch(const json::exception& e) 
    { 
      std::cout << e.what() << std::endl;
      return; 
    }
 
    Request request{j["id"], j["location"], j["latitude"], j["longitude"]};
    match(request, clientService);
  }

  private:
  using RequestEntry = struct { 
      Request request;
      caf::actor clientService;
      std::vector<TaxiType*> taxis; };

  caf::event_based_actor* sender_;
  std::vector<TaxiType> taxis_; 
  std::vector<RequestEntry> requests_;

  void dispatch(TaxiType&, const json&);
  void match(const Request&, caf::actor);
  double calculateTime(const Request&, caf::io::connection_handle);
  void removeGarbage(std::string&);
};

void TaxiServiceCore::match(const Request& request, caf::actor clientService)
{
  try
  {
    RequestEntry entry;
    entry.request = request;
    entry.clientService = clientService;
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

double TaxiServiceCore::calculateTime(const Request& request, 
    caf::io::connection_handle handle)
{
  auto it = std::find_if(taxis_.begin(), taxis_.end(),
      [handle](const TaxiType& taxi)
      {
        return taxi.handle == handle; 
      });

  double distance = distanceEarth(request.latitude, request.longitude, 
    it->data.latitude, it->data.longitude);
  double time = distance / 30.0; 
  
  return time * 60;
}

} // TaxiService