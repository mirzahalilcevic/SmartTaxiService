#pragma once

#include <caf/all.hpp>

#include "service_core.hpp"

namespace taxi_service {

using subscribe_atom = caf::atom_constant<caf::atom("sub")>;
using unsubscribe_atom = caf::atom_constant<caf::atom("unsub")>;

struct service_state
{
  service_core core;
};

caf::behavior service_behavior(caf::stateful_actor<service_state>*);

} // taxi_service
