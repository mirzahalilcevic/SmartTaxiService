#pragma once

#include <caf/all.hpp>
#include <caf/io/all.hpp>
#include <boost/sml.hpp>

#include "Messages.hpp"
#include "ServiceCore.hpp"

namespace ClientService {

class ClientStateTransitions {
  public:
  auto operator()() const noexcept
  {
    using namespace boost::sml;
    return make_transition_table(
    );
  }
};

} // ClientService