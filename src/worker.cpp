#include "worker.hpp"
#include "service.hpp"

namespace server {

using namespace caf;

behavior worker_behavior(worker* self, io::connection_handle handle, 
    caf::actor service_handle)
{
  aout(self) << "fork on new connection" << std::endl;
  self->configure_read(handle, io::receive_policy::at_most(128));
  self->send(service_handle, taxi_service::subscribe_atom::value, handle);
  return {
    
    [=](const io::new_data_msg& msg) 
    {
      aout(self) << to_string(msg) << std::endl;
      self->send(service_handle, msg);
    },

    [=](const io::connection_closed_msg& msg) 
    {
      aout(self) << to_string(msg) << std::endl;
      self->send(service_handle, taxi_service::unsubscribe_atom::value, 
          msg.handle);
      self->quit();
    },

    [=](const io::connection_handle& handle, const std::string& data)
    {
      self->write(handle, data.length(), data.c_str());
      self->flush(handle);
    }

  };
}

} // server
