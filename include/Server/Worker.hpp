#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>

#include "Service/Service.hpp"

namespace Server {

struct WorkerState {};

using Worker = caf::stateful_actor<WorkerState, caf::io::broker>;

caf::behavior workerBehavior(Worker*, caf::io::connection_handle, caf::actor);

} // Server
