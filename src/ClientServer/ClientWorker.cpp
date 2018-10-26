#include "ClientServer/ClientWorker.hpp"

namespace ClientServer {

using namespace caf;

behavior clientWorkerBehavior(ClientWorker* self, io::connection_handle handle, 
    caf::actor service)
{
  self->configure_read(handle, io::receive_policy::at_most(256));
  self->send(service, SubscribeAtom::value, handle); 
  return {
    
    [=](const io::new_data_msg& msg) 
    {
      aout(self) << "[client] " << to_string(msg) << std::endl;
      self->send(service, msg);
    },

    [=](const io::connection_closed_msg& msg) 
    {
      aout(self) << "[client] " << to_string(msg) << std::endl;
      self->send(service, UnsubscribeAtom::value, msg.handle);
      self->quit();
    },

    [=](const std::string& data)
    {
      self->write(handle, data.length(), data.c_str());
      self->flush(handle);
    }

  };
}

} // ClientServer
