#include "service.hpp"

namespace taxi_service {

using namespace caf;

behavior service_behavior(stateful_actor<service_state>* self)
{
  self->state.core.init(self);
  return {
    
    [=](subscribe_atom, caf::io::connection_handle handle)
    {
      self->state.core.subscribe(actor_cast<actor>(self->current_sender()),
          handle);
    },

    [=](unsubscribe_atom, caf::io::connection_handle handle)
    {
      self->state.core.unsubscribe(handle);
    },

    [=](const caf::io::new_data_msg& msg)
    {
      self->state.core.handle(msg);
    }

  };
}

} // taxi_service
