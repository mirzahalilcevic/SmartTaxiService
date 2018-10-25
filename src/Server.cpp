#include "Server.hpp"

namespace Server {

using namespace caf;

behavior serverBehavior(io::broker* self, caf::actor service)
{
  aout(self) << "server up and running" << std::endl;
  return {

    [=](const io::new_connection_msg& msg) 
    {
      aout(self) << to_string(msg) << std::endl;
      auto worker = self->fork(workerBehavior, msg.handle, service);
    }

  };
}

} // Server
