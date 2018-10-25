#include "Server/Worker.hpp"

namespace Server {

using namespace caf;

behavior workerBehavior(Worker* self, io::connection_handle handle, 
    caf::actor service)
{
  self->configure_read(handle, io::receive_policy::at_most(128));
  self->send(service, Service::SubscribeAtom::value, handle);
  return {
    
    [=](const io::new_data_msg& msg) 
    {
      aout(self) << to_string(msg) << std::endl;
      self->send(service, msg);
    },

    [=](const io::connection_closed_msg& msg) 
    {
      aout(self) << to_string(msg) << std::endl;
      self->send(service, Service::UnsubscribeAtom::value, msg.handle);
      self->quit();
    },

    [=](const io::connection_handle& handle, const std::string& data)
    {
      self->write(handle, data.length(), data.c_str());
      self->flush(handle);
    }

  };
}

} // Server
