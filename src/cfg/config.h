#pragma once

#ifndef CLAP_CFG_CONFIG_H
#  define CLAP_CFG_CONFIG_H

namespace clap::config {

extern int verbosity;

void set_verbosity(int level);

} // namespace clap::config

#endif // CLAP_CFG_CONFIG_H
