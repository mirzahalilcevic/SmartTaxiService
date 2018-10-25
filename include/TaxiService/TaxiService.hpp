#pragma once

#include <caf/all.hpp>

#include "TaxiService/TaxiServiceCore.hpp"

namespace TaxiService {

using SubscribeAtom = caf::atom_constant<caf::atom("sub")>;
using UnsubscribeAtom = caf::atom_constant<caf::atom("unsub")>;

struct TaxiServiceState { TaxiServiceCore core; };

caf::behavior taxiServiceBehavior(caf::stateful_actor<TaxiServiceState>*);

} // TaxiService
