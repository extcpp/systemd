#include <ext/systemd.hpp>
#include <thread>
#include <chrono>
#include <iostream>

using namespace std::literals::string_literals;
using namespace std::literals::chrono_literals;


// Simulate a daemon that sends notifications to systemd
int main() {
#ifdef EXT_SYSTEMD_AVAILABLE
    std::cout << "systemd code is active: " << ext::systemd::active() << std::endl;
#else
    std::cout << "systemd support is not even compiled in" << std::endl;
#endif

    // Simulate daemon startup
    std::cout << "starting sample daemon:" << std::endl;
    std::this_thread::sleep_for(3s);
    std::cout << "starting daemon complete" << std::endl;

    // Now we are ready and tell systemd that it can expect notifications
    ext::systemd::notify_ready();
    std::cout << "systemd notified - ready " << std::endl;

    // Send alive notifications in an interval
    for(int i = 0; i < 10; i++) {
        std::this_thread::sleep_for(10s);
        ext::systemd::notify_alive();
        std::cout << "systemd notified - alive " << std::endl;
    }

    // Here we simulate that the daemon is not sending further
    // notifications. Depending on the controlling service
    // it will be restarted.
    std::cout << "now forgetting to notify " << std::endl;
    for (;;) {
        std::this_thread::sleep_for(10s);
        std::cout << "process stuck" << std::endl;
    }

    return 0;
}
