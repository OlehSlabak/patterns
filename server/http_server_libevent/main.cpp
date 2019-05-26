#include "singleton_logging/logging.hpp"
#include "http_server/http_server.hpp"
#include "builder_html/builder_html.h"

#define LOGGING_LEVEL_ALL 1
int main(int argc, char const **argv)
{
    logging::configure({ {"type", "file"}, {"file_name", "test.log"}, {"reopen_interval", "1"} });

   {
        HtmlElement h2("h2", "javaScript test", 1);

        CreatePage(h2);
        h2.Print();
   }





  //  logging::INFO("Start");
 //   Network::Private::HTTPServer Srv(Network::InetAddressV4::CreateFromString("127.0.0.1", 5555), 10, 4, "test_content", "index.html");
   //  std::cin.get();
    /*
    try
    {
       char const serverAddr[] = "Server";
        char const serverPort[] = "Port";
        char const maxBacklog[] = "Backlog";
        char const threadsCount[] = "Threads";
        char const rootDir[] = "Root";
        char const defaultPage[] = "Default Page";

        Common::CommandLine cmdLine(argc, argv);
        //Server:127.0.0.1 Port:5555 Backlog:10 Threads:4 Root:../../test_content DefaultPage:index.html
        Network::Private::HTTPServer Srv(
                Network::InetAddressV4::CreateFromString(
                      cmdLine.GetStrParameter(serverAddr),
                      cmdLine.GetParameter<unsigned short> (serverPort)),
                cmdLine.GetParameter<unsigned> (maxBacklog),
                cmdLine.GetParameter<unsigned>(threadsCount),
                cmdLine.GetStrParameter(rootDir),
                cmdLine.GetStrParameter(defaultPage));

        std::cin.get();
    }
    catch (std::exception const &e)
    {
        logging::ERROR(e.what());
    }*/

    logging::INFO("End");
    return 0;
}
