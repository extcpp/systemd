// Copyright - 2021 - Jan Christoph Uhde <Jan@UhdeJC.com>

#ifndef EXT_SYSTEMD_HEADER
#define EXT_SYSTEMD_HEADER
#include <string>

namespace ext::systemd {
bool active(); // is the feature compiled and active
bool notify_ready();
bool notify_alive();
bool notify_stopping();
bool notify_error(int errno);
bool notify_error(std::string const& reason, int errno);
}

#endif
