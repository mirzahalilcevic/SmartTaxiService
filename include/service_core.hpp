#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>

namespace taxi_service {

struct taxi
{
  using taxi_data = std::vector<char>;
  caf::actor worker;
  caf::io::connection_handle handle;
  taxi_data data;
};

class service_core
{
  public:
    inline void init(caf::event_based_actor* sender)
    {
      sender_ = sender;
    }

    inline void subscribe(caf::actor worker, caf::io::connection_handle handle)
    {
      taxis_.push_back(taxi{worker, handle, {}});
    }

    inline void unsubscribe(caf::io::connection_handle handle)
    {
      auto it = std::find_if(taxis_.cbegin(), taxis_.cend(), 
          [handle](const taxi& t) { return t.handle == handle; });
      if (it == taxis_.cend()) return;

      taxis_.erase(it);
    }

    inline void handle(const caf::io::new_data_msg& msg)
    {
      auto it = std::find_if(taxis_.begin(), taxis_.end(), 
          [msg](const taxi& t) { return t.handle == msg.handle; });
      if (it == taxis_.end()) return;

      it->data = msg.buf;
      
      std::cout << "updated data for taxi: " << caf::to_string(it->handle) << std::endl;
      for (auto el : it->data) std::cout << el;
      std::cout << std::endl;

      std::string str = "received, thank you :)";
      sender_->send(it->worker, msg.handle, str);
    }

  private:
    caf::event_based_actor* sender_;
    std::vector<taxi> taxis_; 
};

} // taxi_service
