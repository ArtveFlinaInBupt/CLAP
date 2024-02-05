#include "cfg/config.h"

namespace clap::config {

int verbosity = 0;

void set_verbosity(int verbosity_level) {
  verbosity = verbosity_level;
}

} // namespace clap::config
