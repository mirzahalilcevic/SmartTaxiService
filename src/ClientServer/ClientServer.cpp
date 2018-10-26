#include "ClientServer/ClientServer.hpp"

namespace ClientServer {

using namespace caf;

behavior clientServerBehavior(io::broker* self, caf::actor service)
{
  aout(self) << "[client] server up and running" << std::endl;
  return {

    [=](const io::new_connection_msg& msg) 
    {
      aout(self) << "[client] " << to_string(msg) << std::endl;
      auto worker = self->fork(clientWorkerBehavior, msg.handle, service);
    }

  };
}

} // ClientServer
