#pragma once

#include <boost/sml.hpp>

#include "Events.hpp"

namespace Service {

class TaxiStateTransitions {
public:
  auto operator()() const noexcept
  {
    using namespace boost::sml;

    auto sendRequest = [](caf::event_based_actor* sender, caf::actor worker,
        caf::io::connection_handle handle, auto event) 
    {
      std::string str = "{ \"request\": { \"location\": " + event.location 
          + ", \"longitude\": " + std::to_string(event.longitude) 
          + ", \"latitude\": " + std::to_string(event.latitude) + "} }";
      sender->send(worker, handle, str);
    };
    
    auto notifyResponse = []() {};

    return make_transition_table(
      
      *"Nromal"_s + event<Request> / sendRequest = "WaitingForResponse"_s,

      "WaitingForResponse"_s + event<Response> / notifyResponse = "Normal"_s

    );
  }
};

} // Service
