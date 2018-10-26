#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>

#include "Messages.hpp"
#include "ClientService/ClientService.hpp"

namespace ClientServer {

struct ClientWorkerState {};

using ClientWorker = caf::stateful_actor<ClientWorkerState, caf::io::broker>;

caf::behavior clientWorkerBehavior(ClientWorker*, caf::io::connection_handle, 
    caf::actor);

} // ClientServer
