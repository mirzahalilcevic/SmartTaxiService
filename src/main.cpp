#include <iostream>
#include <string>

#include <caf/all.hpp>
#include <caf/io/all.hpp>

caf::behavior server(caf::io::broker* self)
{

  return {

    [self](const caf::io::new_connection_msg& msg) 
    {
      caf::aout(self) << "new_connection_msg" << std::endl;
      self->configure_read(msg.handle, caf::io::receive_policy::exactly(5));
    },

    [self](const caf::io::new_data_msg& msg) 
    {
      caf::aout(self) << "new_data_msg" << std::endl;
    },

    [self](const caf::io::data_transferred_msg& msg) 
    {
      caf::aout(self) << "data_transferred_msg" << std::endl;
    },

    [self](const caf::io::connection_closed_msg& msg) 
    {
      caf::aout(self) << "connection_closed_msg" << std::endl;
    },

    [self](const caf::io::acceptor_closed_msg& msg) 
    {
      caf::aout(self) << "acceptor_closed_msg" << std::endl;
    },

    [self](const caf::io::connection_passivated_msg& msg) 
    {
      caf::aout(self) << "connection_passivated_msg" << std::endl;
    },

    [self](const caf::io::acceptor_passivated_msg& msg) 
    {
      caf::aout(self) << "acceptor_passivated_msg" << std::endl;
    }

  };

}

int main(int argc, char const *argv[])
{
  caf::actor_system_config cfg;
  cfg.load<caf::io::middleman>();
  caf::actor_system system{cfg};

  system.middleman().spawn_server(server, 10000); 

  return 0;
}
