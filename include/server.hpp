#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>

namespace server {

caf::behavior server_behavior(caf::io::broker*, caf::actor);

} // server
