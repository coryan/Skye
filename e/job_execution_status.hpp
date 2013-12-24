#ifndef escapment_e_job_execution_status_hpp
#define escapment_e_job_execution_status_hpp

#include <string>
#include <iosfwd>

namespace e {

/**
 * Encapsulates the result of launching a job.
 */
class job_execution_status {
public:
  enum execution_state {
    ES_NOT_STARTED,
    ES_LAUNCHED,
    ES_LAUNCH_FAILED,
    ES_RUNNING,
    ES_COMPLETED,
    ES_COMPLETED_WITH_ERROR,
  };


  job_execution_status(
      std::string const & request_id,
      std::string const & process_id,
      bool successful,
      std::string const & details)
      : request_id_(request_id)
      , process_id_(process_id)
      , successful_(successful)
      , details_(details)
  {}

  /**@{
   * Accessors
   */
  std::string const & request_id() const {
    return request_id_;
  }
  std::string const & process_id() const {
    return process_id_;
  }
  bool successful() const {
    return successful_;
  }
  std::string const & details() const {
    return details_;
  }
  /**@}*/

private:
  std::string request_id_;
  std::string process_id_;
  bool successful_;
  std::string details_;
};

std::ostream & operator<<(std::ostream & os, job_execution_status const & x);

} // namespace e

#endif // escapment_e_job_execution_status_hpp
