// Copyright - 2021 - Jan Christoph Uhde <Jan@UhdeJC.com>

#include <ext/systemd.hpp>

namespace ext { namespace systemd {
#ifdef EXT_SYSTEMD_AVAILABLE

extern "C" {
	#include <systemd/sd-daemon.h>
    #include <string.h>
}

bool active() {
    return true;
}

bool notify_ready(){
    return sd_notify(0 /*unset_environment*/, "READY=1") > 0;
}

bool notify_alive(){
    return sd_notify(0 /*unset_environment*/, "WATCHDOG=1") > 0;
}

bool notify_stopping(){
    return sd_notify(0 /*unset_environment*/, "STOPPING=1") > 0;
}

bool notify_error(int errno){
    return sd_notifyf(0 /*unset_environment*/
                     ,"STATUS=Failed: %s\n"
                      "ERRNO=%i"
                     , strerror(errno), errno
                     ) > 0;
}

bool notify_error(std::string const& reason, int errno){
    // use std::format in the future
    using namespace std::literals::string_literals;
    std::string block = ( "STATUS=Failed: "s + reason );
    block += ( "\nERRNO="s + std::to_string(errno) );

    return sd_notify(0 /*unset_environment*/, block.data()) > 0;
}

#else // not EXT_SYSTEMD_AVAILABLE

bool active() {
    return false;
}

bool notify_ready(){
    return true;
}

bool notify_alive(){
    return true;
}

bool notify_stopping(){
    return true;
}

bool notify_error(int errno){
    return true;
}

bool notify_error(std::string const& reason, int errno){
    return true;
}

#endif // EXT_SYSTEMD_AVAILABLE

}} // namespace ext::systemd
