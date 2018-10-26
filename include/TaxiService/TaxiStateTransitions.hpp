#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>
#include <boost/sml.hpp>

#include "Messages.hpp"
#include "Tools/json.hpp"
#include "ServiceCore.hpp"

namespace TaxiService {

using json = nlohmann::json;

class TaxiStateTransitions {
  public:
  auto operator()() const noexcept
  {
    using namespace boost::sml;

    auto sendRequest = [](ServiceCore* core, caf::io::connection_handle handle,
        auto event) 
    {
      json request;
      request["type"] = "request";
      request["location"] = event.location;
      request["latitude"] = event.latitude;
      request["longitude"] = event.longitude;

      core->send(core->getWorker(handle), request.dump());
    };
    
    auto notifyResponse = [](auto event)
    {
    };

    return make_transition_table(
      *"Nromal"_s + event<Request> / sendRequest = "WaitingForResponse"_s,
      "WaitingForResponse"_s + event<Response> / notifyResponse = "Normal"_s
    );
  }
};

} // TaxiService
