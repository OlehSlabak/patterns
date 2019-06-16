#include "singleton_logging/logging.hpp"
#include "http_server/http_server.hpp"
#include "builder_html/builder_html.h"

#define LOGGING_LEVEL_ALL 1
int main(int argc, char const **argv)
{
    logging::configure({ {"type", "file"}, {"file_name", "test.log"}, {"reopen_interval", "1"} });

    /* Builder #1 */
    Builder_1::HtmlElement h2("h2", "teg test", 0);
    Builder_1::TextElement text("h2", "body text");
    Builder_1::CreatePage(h2);
    Builder_1::CreatePage(text);

    h2.Print();
    text.Print();

    std::cout << "\n\n" << std::endl;
    /* Builder #2 */
    Builder_2::HtmlBuilder builder { "ul" };
    builder.add_child("li", "hello").add_child("li", "world");
    std::cout << builder.str() << std::endl;

    std::cout << "\n\n" << std::endl;
    /* Builder_3 */
    Builder_3::Product p1 = Builder_3::Product::Builder().setI(2).setF(0.5f).setC('x').setI(3).build();
	p1.print();

	Builder_3::Product::Builder b;
	b.setProductP();

	Builder_3::Product p2 = b.build(); // get Product P object
	b.setC('!'); // customize Product P

	Builder_3::Product p3 = b.build();
	p2.print(); // test p2
	p3.print(); // test p3



    logging::INFO("Start HTTP Server");
    Network::Private::HTTPServer Srv(Network::InetAddressV4::CreateFromString("127.0.0.1", 5555), 10, 4, "test_content", "index.html");
    std::cin.get();
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

    logging::INFO("Stop HTTP Server");
    return 0;
}
