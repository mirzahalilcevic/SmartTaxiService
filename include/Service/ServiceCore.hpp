#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>

namespace Service {

class ServiceCore
{
  public:
    inline void init(caf::event_based_actor* sender)
    {
      sender_ = sender;
    }

    inline void subscribe(caf::actor worker, caf::io::connection_handle handle)
    {
      activeTaxis_.push_back(Taxi{worker, handle, {}});
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

      it->data = msg.buf;
      
      std::cout << "updated data for taxi: " << caf::to_string(it->handle) 
          << std::endl;
      for (auto el : it->data) std::cout << el;
      std::cout << std::endl;

      std::string str = "received, thank you :)";
      sender_->send(it->worker, msg.handle, str);
    }

  private:
    struct Taxi
    {
      using TaxiData = std::vector<char>;
      caf::actor worker;
      caf::io::connection_handle handle;
      TaxiData data;
    };

    caf::event_based_actor* sender_;
    std::vector<Taxi> activeTaxis_; 
};

} // Service
