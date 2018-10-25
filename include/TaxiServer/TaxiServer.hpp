#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>

#include "TaxiServer/TaxiWorker.hpp"

namespace TaxiServer {

caf::behavior taxiServerBehavior(caf::io::broker*, caf::actor);

} // TaxiServer
