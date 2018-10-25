#include "Server/Server.hpp"
#include "Service/Service.hpp"

int main(int argc, char const *argv[])
{
  if (argc < 2)
  {
    std::cerr << "*** ERROR  no port number specified" << std::endl;
    return -1;
  }

  caf::actor_system_config cfg;
  cfg.load<caf::io::middleman>();
  caf::actor_system system{cfg};

  auto service = system.spawn(Service::serviceBehavior);
  system.middleman().spawn_server(Server::serverBehavior, std::stoi(argv[1]), 
      service);

  return 0;
}
