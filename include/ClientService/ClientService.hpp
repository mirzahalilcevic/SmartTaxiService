#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>

#include "Messages.hpp"
#include "ClientService/ClientServiceCore.hpp"

namespace ClientService {

struct ClientServiceState { ClientServiceCore core; };

caf::behavior clientServiceBehavior(caf::stateful_actor<ClientServiceState>*);

} // ClientService
