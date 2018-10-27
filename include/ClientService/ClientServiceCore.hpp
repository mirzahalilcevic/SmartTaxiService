#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>
#include <boost/sml.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

#include "ServiceCore.hpp"
#include "Messages.hpp"
#include "Tools/json.hpp"
#include "ClientService/ClientStateTransitions.hpp"

namespace ClientService {

using json = nlohmann::json;

class ClientServiceCore : public ServiceCore
{
  public:
  struct ClientType
  {
    using StateMachine = boost::sml::sm<ClientStateTransitions>;

    caf::actor worker;
    caf::io::connection_handle handle;
    std::unique_ptr<StateMachine> stateMachine;
    Request request;

    ClientType(ServiceCore* core, caf::actor w,
        caf::io::connection_handle h)
      : worker{w}, handle{h}, 
        stateMachine{std::make_unique<StateMachine>(core, handle)} {}
  };
  
  inline void init(caf::event_based_actor* sender, caf::actor taxiService)
  {
    sender_ = sender;
    taxiService_ = taxiService;
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
    json j;
  	try 
    {
      j = json::parse(msg.buf); 
    } 
    catch(const json::exception& e) 
    { 
      std::cout << e.what() << std::endl;
      return; 
    }
    
    auto it = std::find_if(clients_.begin(), clients_.end(), 
        [msg](const ClientType& client)
        { 
          return client.handle == msg.handle; 
        });

    if (it == clients_.end()) return;

    if (j["type"] == "request")
    {
      boost::uuids::uuid id = gen_();
      Request request{boost::lexical_cast<std::string>(id), j["location"],
          j["latitude"], j["longitude"]};
      it->request = request;
      it->stateMachine->process_event(request);
    }
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

  inline caf::actor getTaxiService() override
  {
    return taxiService_;
  }

  inline bool isFirst(caf::io::connection_handle handle, 
      Response response) override
  {
    // nop
  }

  inline bool isLast(caf::io::connection_handle handle,
      Response response) override
  {
    // nop
  }

  inline void removeEntry(caf::io::connection_handle handle,
      Response response) override
  {
    // nop
  }

  inline void sendResponse(const Response& response)
  {
    auto it = std::find_if(clients_.begin(), clients_.end(),
        [response](const ClientType& client)
        {
          return client.request.id == response.id;
        });

    if (it == clients_.end()) return;

    it->stateMachine->process_event(response);
  }

  private:
  boost::uuids::basic_random_generator<boost::mt19937> gen_;
  caf::event_based_actor* sender_;
  std::vector<ClientType> clients_; 
  caf::actor taxiService_;
};

} // ClientService
