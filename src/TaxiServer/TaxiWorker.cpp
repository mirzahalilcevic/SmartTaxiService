#include "TaxiServer/TaxiWorker.hpp"

namespace TaxiServer {

using namespace caf;

behavior taxiWorkerBehavior(TaxiWorker* self, io::connection_handle handle, 
    caf::actor service)
{
  self->configure_read(handle, io::receive_policy::at_most(256));
  self->send(service, SubscribeAtom::value, handle);
  return {
    
    [=](const io::new_data_msg& msg) 
    {
      aout(self) << "[taxi] " << to_string(msg) << std::endl;
      self->send(service, msg);
    },

    [=](const io::connection_closed_msg& msg) 
    {
      aout(self) << "[taxi] " << to_string(msg) << std::endl;
      self->send(service, UnsubscribeAtom::value, msg.handle);
      self->quit();
    },

    [=](const std::string& data)
    {
      self->write(handle, data.length(), (const void *) data.c_str());
      self->flush(handle);
    }

  };
}

} // TaxiServer
