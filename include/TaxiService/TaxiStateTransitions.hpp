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
      request["id"] = event.id;
      request["location"] = event.location;
      request["latitude"] = event.latitude;
      request["longitude"] = event.longitude;

      core->send(core->getWorker(handle), request.dump());
    };
    
    auto notifyAccept = [](ServiceCore* core, caf::io::connection_handle handle,
        auto event)
    {
      json accept;
      accept["type"] = "accept";
      accept["id"] = event.id;

      core->send(core->getWorker(handle), accept.dump());
    };

    auto notifyReject = [](ServiceCore* core, caf::io::connection_handle handle,
        auto event)
    {
      json reject;
      reject["type"] = "reject";
      reject["id"] = event.id;

      core->send(core->getWorker(handle), reject.dump());
    };

    auto isFirst = [](ServiceCore* core, caf::io::connection_handle handle,
        auto event)
    {
      return core->isFirst(handle, event.id);
    };

    auto isAccept = [](auto event)
    {
      return event.accept;
    };

    return make_transition_table(

      *"Normal"_s + event<Request> / sendRequest = "WaitingForResponse"_s,

      "WaitingForResponse"_s + event<Response>[isFirst && isAccept] / notifyAccept = "Normal"_s,

      "WaitingForResponse"_s + event<Response>[!isFirst && isAccept] / notifyReject = "Normal"_s,

      "WaitingForResponse"_s + event<Response>[!isAccept] = "Normal"_s,

      "WaitingForResponse"_s + event<Cancel> / notifyReject = "Normal"_s 

    );
  }
};

} // TaxiService
