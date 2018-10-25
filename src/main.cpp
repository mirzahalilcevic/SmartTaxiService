#include "TaxiServer/TaxiServer.hpp"
#include "TaxiService/TaxiService.hpp"

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

  auto service = system.spawn(TaxiService::taxiServiceBehavior);
  system.middleman().spawn_server(TaxiServer::taxiServerBehavior,
      std::stoi(argv[1]), service);

  return 0;
}
