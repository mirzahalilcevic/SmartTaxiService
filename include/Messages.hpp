#pragma once

#include <caf/all.hpp>
#include <string>

namespace {

using SubscribeAtom = caf::atom_constant<caf::atom("sub")>;
using UnsubscribeAtom = caf::atom_constant<caf::atom("unsub")>;

struct Request
{
  std::string id;
  std::string location;
  double latitude;
  double longitude;
};

struct Response
{
  std::string id;
  bool accept;
};

struct Cancel
{
  std::string id;
};

} // anonymous namespace
