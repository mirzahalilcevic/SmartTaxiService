#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>

namespace server {

struct worker_state {};

using worker = caf::stateful_actor<worker_state, caf::io::broker>;

caf::behavior worker_behavior(worker*, caf::io::connection_handle, caf::actor);

} // server
