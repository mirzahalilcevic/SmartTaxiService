#include "TaxiService/TaxiService.hpp"

namespace TaxiService {

using namespace caf;

behavior taxiServiceBehavior(stateful_actor<TaxiServiceState>* self)
{
  self->state.core.init(self);
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

    [=](const std::string& msg)
    {
      self->state.core.sendRequest(msg,
          actor_cast<actor>(self->current_sender()));
    }

  };
}

} // TaxiService
