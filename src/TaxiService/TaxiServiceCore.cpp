#include "TaxiService/TaxiServiceCore.hpp"

namespace TaxiService {

void TaxiServiceCore::dispatch(TaxiType& taxi, const json& j)
{
  if (j["type"] == "location")
  {
    taxi.data.latitude = j["latitude"];
    taxi.data.longitude = j["longitude"];
    taxi.isLocationReceived = true;
  }
  else if (j["type"] == "response")
  {
    Response response{j["id"], j["accept"], 0.0};
    taxi.stateMachine->process_event(response);
  }
}

void blockingThread(std::function<void(const boost::system::error_code&)> f)
{
  boost::asio::io_service io;
  boost::asio::deadline_timer t{io, boost::posix_time::seconds(10)};
  t.async_wait(f);
  io.run();
}

} // TaxiService
