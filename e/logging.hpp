#ifndef escapement_e_logging_hpp
#define escapement_e_logging_hpp

//#include <boost/log/trivial.hpp>
#include <iostream>

// TODO(coryan): right now we are using trivial logging, eventually we
// will want to setup some kind of singleton or something even more
// clever.
#define E_LOG(sev) std::cerr /*BOOST_LOG_TRIVIAL(sev)*/

#endif // escapement_e_logging_hpp
