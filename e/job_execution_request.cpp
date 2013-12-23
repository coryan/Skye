#include <e/job_execution_request.hpp>
#include <iostream>

std::ostream & e::operator<<(
    std::ostream & os, job_execution_request const & x) {
  return os << "request.id=" << x.id() << ", request.spec=" << x.job();
}
