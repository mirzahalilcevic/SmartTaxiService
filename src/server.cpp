#include "server.hpp"
#include "worker.hpp"

namespace server {

using namespace caf;

behavior server_behavior(io::broker* self, caf::actor service_handle)
{
  aout(self) << "server up and running" << std::endl;
  return {

    [=](const io::new_connection_msg& msg) 
    {
      aout(self) << to_string(msg) << std::endl;
      auto worker = self->fork(worker_behavior, msg.handle, service_handle);
    }

  };
}

} // server
