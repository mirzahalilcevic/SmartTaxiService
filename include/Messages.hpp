#pragma once

#include <caf/all.hpp>
#include <string>

namespace {

using SubscribeAtom = caf::atom_constant<caf::atom("sub")>;
using UnsubscribeAtom = caf::atom_constant<caf::atom("unsub")>;
using CancelAtom = caf::atom_constant<caf::atom("cancel")>;

struct Request
{
  std::string location;
  double latitude;
  double longitude;
};

struct Response
{
  bool accept;
};

} // anonymous namespace
