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
  typedef std::vector<stringT> arguments;
  typedef std::map<stringT,stringT> environment;

  explicit job_spec_base(stringT const & executable)
    : executable_(executable)
  {}

  //@{
  // Accessors, read the different fields
  stringT const & executable() const {
    return executable_;
  }
  arguments const & args() const {
    return args_;
  }
  environment const & env() const {
    return env_;
  }
  stringT const & wd() const {
    return wd_;
  }
  stringT const & stdout() const {
    return stdout_;
  }
  stringT const & stderr() const {
    return stderr_;
  }
  stringT const & username() const {
    return username_;
  }
  stringT const & groupname() const {
    return groupname_;
  }
  //@}

  //@{
  // Modifiers, they all return a reference to the object so they can
  // be chained
  job_spec_base& set_args(arguments const & args) {
    arguments tmp(args);
    tmp.swap(args_);
    return *this;
  }

  job_spec_base& set_env(environment const & env) {
    environment tmp(env);
    tmp.swap(env_);
    return *this;
  }
  job_spec_base& set_wd(stringT const & wd) {
    return set_field(wd_, wd);
  }
  job_spec_base& set_stdout(stringT const & filename) {
    return set_field(stdout_, filename);
  }
  job_spec_base& set_stderr(stringT const & filename) {
    return set_field(stderr_, filename);
  }
  job_spec_base& set_username(stringT const & username) {
    return set_field(username_, username);
  }
  job_spec_base& set_groupname(stringT const & groupname) {
    return set_field(groupname_, groupname);
  }

private:
  job_spec_base& set_field(stringT & field, stringT const & value) {
    stringT tmp(value);
    tmp.swap(field);
    return *this;
  }

private:
  stringT executable_;
  arguments args_;
  environment env_;
  stringT wd_;
  stringT stdout_;
  stringT stderr_;
  stringT username_;
  stringT groupname_;
};

typedef job_spec_base<std::string> job_spec;
typedef job_spec_base<std::wstring> wjob_spec;

} // namespace e

#endif // escapment_lib_e_job_spec_hpp
