#include <e/job_spec.hpp>
#include <boost/algorithm/string/join.hpp>
#include <iostream>

std::ostream & e::operator<<(std::ostream & os, job_spec const & x) {
  os << "spec.executable=" << x.executable()
     << ", spec.args={"
     << boost::algorithm::join(x.args(), ", ")
     << "}, spec.env={";

  bool first = true;
  for(auto const & i : x.env()) {
    if (not first) {
      os << ", ";
    }
    os << i.first << "=" << i.second;
    first = false;
  }

  os << "}";
  return os;
}


