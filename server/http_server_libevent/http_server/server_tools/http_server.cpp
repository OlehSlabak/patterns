#include "../http_server.hpp"

namespace Network
{
    namespace Private
    {
        HttpEventHolder::HttpEventHolder(SocketHandle sock, _requestHandler const &handler)
        : _handler(handler), _http(evhttp_new(GetBase()))
        {
            evhttp_set_allowed_methods(_http, EVHTTP_REQ_GET);
            evhttp_set_gencb(_http, &HttpEventHolder::RawHttpRequestHandler, this);
            if (evhttp_accept_socket(_http, sock) == -1)
                throw HttpEventHolderException("Failed to accept socket for http");
        }

        void HttpEventHolder::ProcessRequest(evhttp_request *request)
        {
            try {
                _handler(request->uri, request->output_buffer);
                evhttp_send_reply(request, HTTP_OK, "OK", request->output_buffer);

            }
            catch (std::exception const &e)
            {
                evhttp_send_reply(request, HTTP_INTERNAL, e.what() ? e.what() : "Internal server error.", request->output_buffer);
            }
        }

        void ServerThread::DispatchProc()
        {
            _isRun = true;
            while (_isRun)
            {
                if (event_base_loop(GetBase(), EVLOOP_NONBLOCK))
                {
                    logging::ERROR("Failed to run dispatch events");
                    break;
                }
                usleep(WaitTimeout);
            }
        }

        void ServerThread::OnRequest(char const *resource, evbuffer *outBuffer)
        {
            std::string fileName;
            GetFullFileName(resource, &fileName);
            try
            {
                System::FileHolder file(fileName);

                if (!file.GetSize())
                {
                    evbuffer_add_printf(outBuffer, "Empty file");
                    return;
                }

                evbuffer_add_file(outBuffer, file.GetHandle(), 0, file.GetSize());
                file.Detach();
            }
            catch (System::FileHolderException const &)
            {
                evbuffer_add_printf(outBuffer, "File not found");
            }
        }

        void ServerThread::GetFullFileName(char const *resource, std::string *fileName) const
        {
            fileName->append(_rootDir);
            if (!resource || !strcmp(resource, "/"))
            {
                fileName->append("/");
                fileName->append(_defaultPage);
            }
            else
            {
               fileName->append(resource);
            }
        }

        /*:  TCPServerSocket(locAddr, backlog); */
        HTTPServer::HTTPServer(Network::InetAddress const &locAddr, int backlog, int maxThreadCount, std::string const &rootDir, std::string const &defaultPage)
        : TCPServerSocket(locAddr, backlog)
        {
        logging::INFO("HTTPServer");
            for (int i = 0; i < maxThreadCount; ++i)
            {
                _serverThreads.push_back(_serverThreadPtr(new Private::ServerThread(GetHandler(), rootDir, defaultPage)));
            }
        }
    }
}
