#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>

#include "TaxiService/TaxiStateTransitions.hpp" 

namespace TaxiService {

struct Taxi
{
  using TaxiData = struct { double latitude; double longitude;};
  using StateMachine = boost::sml::sm<TaxiStateTransitions>;

  caf::actor worker;
  caf::io::connection_handle handle;
  TaxiData data;
  std::unique_ptr<StateMachine> stateMachine;
	bool isLocationReceived = false;

  Taxi(caf::event_based_actor* sender, caf::actor wrkr, 
      caf::io::connection_handle hndl)
    : worker{wrkr}, handle{hndl}, 
      stateMachine{std::make_unique<StateMachine>(sender, wrkr, hndl)} {}
};

class TaxiServiceCore
{
  public:
    inline void init(caf::event_based_actor* sender)
    {
      sender_ = sender;
    }

    inline void subscribe(caf::actor worker, caf::io::connection_handle handle)
    {
      activeTaxis_.push_back(Taxi{sender_, worker, handle});
    }

    inline void unsubscribe(caf::io::connection_handle handle)
    {
      auto it = std::find_if(activeTaxis_.cbegin(), activeTaxis_.cend(), 
          [handle](const Taxi& t) { return t.handle == handle; });
      if (it == activeTaxis_.cend()) return;

      activeTaxis_.erase(it);
    }

    inline void handle(const caf::io::new_data_msg& msg)
    {
      auto it = std::find_if(activeTaxis_.begin(), activeTaxis_.end(), 
          [msg](const Taxi& t) { return t.handle == msg.handle; });
      if (it == activeTaxis_.end()) return;

      dispatch(*it, msg.buf);
    }

    inline bool sendRequest(const Request& request)
    {
			try
      {
      	auto& taxi = getNearestTaxi(request);
      	taxi.stateMachine->process_event(request);
				return true;
			}
      catch (const std::logic_error e)
      {
				return false;
			}
    }

  private:
    caf::event_based_actor* sender_;
    std::vector<Taxi> activeTaxis_; 

    void dispatch(Taxi&, const std::vector<char>&);
    Taxi& getNearestTaxi(const Request&);
};

} // TaxiService
