#include "Service/Service.hpp"

namespace Service {

using namespace caf;

behavior serviceBehavior(stateful_actor<ServiceState>* self)
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
    }

  };
}

} // Service
