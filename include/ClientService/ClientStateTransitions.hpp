#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>
#include <boost/sml.hpp>

#include "Messages.hpp"
#include "ServiceCore.hpp"
#include "Tools/json.hpp"

namespace ClientService {

using json = nlohmann::json;

class ClientStateTransitions {
  public:
  auto operator()() const noexcept
  {
    using namespace boost::sml;

    auto sendRequest = [](ServiceCore* core, caf::io::connection_handle handle,
        auto event)
    {
      json request;
      request["type"] = "request";
      request["id"] = event.id;
      request["location"] = event.location;
      request["latitude"] = event.latitude;
      request["longitude"] = event.longitude;

      std::cout << "[client] " << request.dump() << std::endl;
      core->send(core->getTaxiService(), request.dump());
    };

    auto notifyResponse = [](ServiceCore* core,
      caf::io::connection_handle handle, auto event)
    {
      json response;
      response["type"] = "response";
      response["accept"] = event.accept;
      response["latitude"] = event.latitude;
      response["longitude"] = event.longitude;

      std::cout << "[client] " << response.dump() << std::endl;
      core->send(core->getWorker(handle), response.dump());
    };

    return make_transition_table(
      
      *"Normal"_s + event<Request> / sendRequest = "WaitingForResponse"_s,

      "WaitingForResponse"_s + event<Response> / notifyResponse = "Normal"_s

    );
  }
};

} // ClientService