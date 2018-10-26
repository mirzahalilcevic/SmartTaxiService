#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>
#include <boost/sml.hpp>

#include "ServiceCore.hpp"
#include "ClientService/ClientStateTransitions.hpp"

namespace ClientService {

class ClientServiceCore : public ServiceCore
{
  public:
  struct ClientType
  {
    using StateMachine = boost::sml::sm<ClientStateTransitions>;

    caf::actor worker;
    caf::io::connection_handle handle;
    std::unique_ptr<StateMachine> stateMachine;

    ClientType(ClientServiceCore* core, caf::actor w,
        caf::io::connection_handle h)
      : worker{w}, handle{h}, 
        stateMachine{std::make_unique<StateMachine>(core, handle)} {}
  };
  
  inline void init(caf::event_based_actor* sender)
  {
    sender_ = sender;
  }

  inline void subscribe(caf::actor worker, caf::io::connection_handle handle)
  {
    clients_.push_back(ClientType{this, worker, handle});
  }

  inline void unsubscribe(caf::io::connection_handle handle)
  {
    auto it = std::find_if(clients_.cbegin(), clients_.cend(), 
        [handle](const ClientType& client)
        { 
          return client.handle == handle; 
        });
    
    if (it == clients_.cend())
      return;
    else
      clients_.erase(it);
  }

  inline void handle(const caf::io::new_data_msg& msg)
  {
    // TODO
  }
  
  inline void send(caf::actor worker, const std::string& data) override
  {
    sender_->send(worker, data);
  }

  inline caf::actor getWorker(caf::io::connection_handle handle) override
  {
    auto it = std::find_if(clients_.begin(), clients_.end(), 
        [handle](const ClientType& client)
        { 
          return client.handle == handle; 
        });

    if (it != clients_.end()) 
      return it->worker;
    else
      throw std::logic_error{"client not found"};
  }

  private:
  caf::event_based_actor* sender_;
  std::vector<ClientType> clients_; 
};

} // ClientService
