#include <e/job_execution_status.hpp>
#include <iostream>

std::ostream & e::operator<<(
    std::ostream & os, job_execution_status const & x) {
  return os << "request_id=" << x.request_id()
            << ", process_id=" << x.process_id()
            << ", successful=" << x.successful()
            << ", details=" << x.details();
}
