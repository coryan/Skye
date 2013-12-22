#ifndef escapment_lib_e_job_spec_hpp
#define escapment_lib_e_job_spec_hpp

#include <vector>
#include <map>
#include <string>

namespace e {

template<typename stringT>
class job_spec_base
{
public:
  explicit job_spec_base(stringT const & executable)
    : executable_(executable)
  {}

  //@{
  // Accessors, read the different fields
  stringT const & executable() const {
    return executable_;
  }
  std::vector<stringT> const & args() const {
    return args_;
  }
  std::map<stringT,stringT> const & env() const {
    return env_;
  }
  stringT const & stdout() const {
    return stdout_;
  }
  //@}

  job_spec_base& set_args(std::vector<stringT> const & args) {
    std::vector<stringT> tmp(args);
    tmp.swap(args_);
    return *this;
  }

  job_spec_base& set_environment(std::map<stringT,stringT> const & env) {
    std::map<stringT,stringT> tmp(env);
    tmp.swap(env_);
    return *this;
  }
  job_spec_base& set_stdout_basename(stringT const & filename);
  job_spec_base& set_stderr_basename(stringT const & filename);
  job_spec_base& set_username(stringT const & username);
  job_spec_base& set_groupname(stringT const & groupname);

private:
  stringT executable_;
  std::vector<stringT> args_;
  std::map<stringT,stringT> env_;
  stringT stdout_;
  stringT stderr_;
  stringT username_;
  stringT groupname_;
};

typedef job_spec_base<std::string> job_spec;
typedef job_spec_base<std::wstring> wjob_spec;

} // namespace e

#endif // escapment_lib_e_job_spec_hpp
