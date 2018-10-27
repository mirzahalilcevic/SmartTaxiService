#include "ClientService/ClientService.hpp"

namespace ClientService {

using namespace caf;

behavior clientServiceBehavior(stateful_actor<ClientServiceState>* self,
    caf::actor taxiService)
{
  self->state.core.init(self, taxiService);
  return {
    
    [=](SubscribeAtom, caf::io::connection_handle handle)
    {
      self->state.core.subscribe(actor_cast<actor>(self->current_sender()),
          handle);
    },

    [=](UnsubscribeAtom, caf::io::connection_handle handle)
    {
      self->state.core.unsubscribe(handle);
    },

    [=](const caf::io::new_data_msg& msg)
    {
      self->state.core.handle(msg);
    },

    [=](const Response& response)
    {
      self->state.core.sendResponse(response);
    }

  };
}

} // ClientService
