#include "e/config.hpp"

// This is an ugly mess of #ifdefs, but better here than sprinkled
// around in the code.

// TODO(coryan) provide good platform-specific defaults
#if !defined(ESCAPEMENT_DEFAULT_SYSCONFDIR)
#  define ESCAPEMENT_DEFAULT_SYSCONFDIR "/etc"
#endif // ESCAPEMENT_DEFAULT_SYSCONFDIR
#if !defined(ESCAPEMENT_DEFAULT_BINDIR)
#  define ESCAPEMENT_DEFAULT_BINDIR "/usr/bin"
#endif // ESCAPEMENT_DEFAULT_BINDIR

char const * e::sysconfdir() {
#if defined(ESCAPEMENT_SYSCONFDIR)
  return ESCAPEMENT_SYSCONFDIR;
#endif // ESCAPEMENT_SYSCONFDIR
  return ESCAPEMENT_DEFAULT_SYSCONFDIR;
}

char const * e::bindir() {
#if defined(ESCAPEMENT_BINDIR)
  return ESCAPEMENT_BINDIR;
#endif // ESCAPEMENT_BINDIR
  return ESCAPEMENT_DEFAULT_BINDIR;
}
