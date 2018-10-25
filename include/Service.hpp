#pragma once

#include <caf/all.hpp>

#include "ServiceCore.hpp"

namespace Service {

using SubscribeAtom = caf::atom_constant<caf::atom("sub")>;
using UnsubscribeAtom = caf::atom_constant<caf::atom("unsub")>;

struct ServiceState { ServiceCore core; };

caf::behavior serviceBehavior(caf::stateful_actor<ServiceState>*);

} // Service
