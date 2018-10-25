#include "TaxiServer/TaxiServer.hpp"

namespace TaxiServer {

using namespace caf;

behavior taxiServerBehavior(io::broker* self, caf::actor service)
{
  aout(self) << "server up and running" << std::endl;
  return {

    [=](const io::new_connection_msg& msg) 
    {
      aout(self) << to_string(msg) << std::endl;
      auto worker = self->fork(taxiWorkerBehavior, msg.handle, service);
    }

  };
}

} // TaxiServer
