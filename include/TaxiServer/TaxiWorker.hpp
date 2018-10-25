#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>
#include <boost/sml.hpp>

#include "TaxiService/TaxiService.hpp"

namespace TaxiServer {

struct TaxiWorkerState {};

using TaxiWorker = caf::stateful_actor<TaxiWorkerState, caf::io::broker>;

caf::behavior taxiWorkerBehavior(TaxiWorker*, caf::io::connection_handle, 
    caf::actor);

} // TaxiServer
