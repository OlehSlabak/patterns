#include <iostream>
#include <event.h>
#include <evhttp.h>

#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <vector>
#include <functional>
#include <memory>


#include "logging.hpp"

//#include "tcp_server_socket.h"
//#include "inet_address_v4.h"
#include <thread>
//#include "command_line.h"
//#include "logger.h"
//#include "file_holder.h"


using namespace std;

int main()
{

    logging::configure({ {"type", "file"}, {"file_name", "test.log"}, {"reopen_interval", "1"} });

    logging::INFO("Start");

    /*Run http server */

    logging::INFO("End");
    return 0;
}
