#pragma once

#include <string>
#include <caf/io/all.hpp>

namespace {

class ServiceCore
{
  public:
  virtual void send(caf::actor, const std::string&) = 0;
  virtual caf::actor getWorker(caf::io::connection_handle) = 0;
  virtual ~ServiceCore() = default;
};

} // anonymous namespace
