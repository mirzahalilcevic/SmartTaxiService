#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>

#include "ClientServer/ClientWorker.hpp"

namespace ClientServer {

caf::behavior clientServerBehavior(caf::io::broker*, caf::actor);

} // ClientServer
