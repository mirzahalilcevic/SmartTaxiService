#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>

#include "Worker.hpp"

namespace Server {

caf::behavior serverBehavior(caf::io::broker*, caf::actor);

} // Server