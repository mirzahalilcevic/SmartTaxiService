#pragma once

#include <string>
#include <caf/io/all.hpp>

#include "Messages.hpp"

namespace {

class ServiceCore
{
  public:
  virtual void send(caf::actor, const std::string&) = 0;
  virtual caf::actor getWorker(caf::io::connection_handle) = 0;
  virtual caf::actor getTaxiService() = 0;
  virtual bool isFirst(caf::io::connection_handle, Response) = 0;
  virtual bool isLast(caf::io::connection_handle, Response) = 0;
  virtual void removeEntry(caf::io::connection_handle, Response) = 0;
  virtual ~ServiceCore() = default;
};

} // anonymous namespace
