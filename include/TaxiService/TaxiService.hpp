#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>

#include "Messages.hpp"
#include "TaxiService/TaxiServiceCore.hpp"

namespace TaxiService {

struct TaxiServiceState { TaxiServiceCore core; };

caf::behavior taxiServiceBehavior(caf::stateful_actor<TaxiServiceState>*);

} // TaxiService
