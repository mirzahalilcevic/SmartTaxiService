#include "TaxiServer/TaxiServer.hpp"
#include "TaxiService/TaxiService.hpp"
#include "ClientServer/ClientServer.hpp"
#include "ClientService/ClientService.hpp"

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    std::cerr << "*** ERROR  no port number specified" << std::endl;
    return -1;
  }

  caf::actor_system_config cfg;
  cfg.load<caf::io::middleman>();
  caf::actor_system system{cfg};

  auto taxiService = system.spawn(TaxiService::taxiServiceBehavior);
  system.middleman().spawn_server(TaxiServer::taxiServerBehavior,
      std::stoi(argv[1]), taxiService);

  auto clientService = system.spawn(ClientService::clientServiceBehavior,
      taxiService);
  system.middleman().spawn_server(ClientServer::clientServerBehavior,
      std::stoi(argv[2]), clientService);

  system.await_all_actors_done();
  return 0;
}
