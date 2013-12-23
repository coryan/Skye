#ifndef escapment_e_job_execution_request_hpp
#define escapment_e_job_execution_request_hpp

#include <e/job_spec.hpp>

#include <utility>
#include <string>

namespace e {

/**
 * Encapsulates a request to launch a job.
 */
class job_execution_request
{
public:
  job_execution_request(
      std::string const & id,
      job_spec const & job)
      : id_(id)
      , job_(job)
  {}

  std::string const & id() const {
    return id_;
  }
  job_spec const & job() const {
    return job_;
  }

  bool operator<(job_execution_request const & rhs) const {
    return id_ < rhs.id_;
  }
  bool operator==(job_execution_request const & rhs) const {
    return id_ == rhs.id_;
  }

private:
  std::string id_;
  job_spec job_;
};

std::ostream & operator<<(std::ostream & os, job_execution_request const & x);

} // namespace e

#endif // escapment_e_job_execution_request_hpp
