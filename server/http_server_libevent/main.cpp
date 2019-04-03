#include "singleton_logging/logging.hpp"
#include "http_server/http_server.hpp"

int main()
{

    logging::configure({ {"type", "file"}, {"file_name", "test.log"}, {"reopen_interval", "1"} });

    logging::INFO("Start");

    /*Run http server */

    logging::INFO("End");
    return 0;
}
