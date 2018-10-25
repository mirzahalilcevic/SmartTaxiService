#include "server.hpp"
#include "service.hpp"

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

  auto service_handle = system.spawn(taxi_service::service_behavior);
  system.middleman().spawn_server(server::server_behavior, std::stoi(argv[1]), 
      service_handle);

  return 0;
}
