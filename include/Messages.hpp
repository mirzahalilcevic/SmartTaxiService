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
  double latitude;
  double longitude;
};

struct Cancel
{
  std::string id;
};

template <class Inspector>
typename Inspector::result_type inspect(Inspector& f, Response& r) {
  return f(caf::meta::type_name("Response"), r.id, r.accept, r.latitude, 
      r.longitude);
}

} // anonymous namespace
