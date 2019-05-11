#ifndef HTTP_SERVER_HPP_INCLUDED
#define HTTP_SERVER_HPP_INCLUDED

#include "../singleton_logging/logging.hpp"

#include <event.h>
#include <evhttp.h>
#include <unistd.h>
#include <signal.h>
#include <vector>
#include <functional>
#include <memory>
#include <functional>
#include <string.h>

#include "server_tools/tcp_server_socket.h"
#include "server_tools/inet_address_v4.h"
#include "server_tools/command_line.h"
#include "server_tools/file_holder.h"
#include "server_tools/thread.hpp"

/* */
namespace Network
{
    namespace Private
    {
        DECLARE_RUNTIME_EXCEPTION(EventBaseHolder)
        DECLARE_RUNTIME_EXCEPTION(HttpEventHolder)

        class EventBaseHolder : private Common::NonCopyable
        {
            public:
                EventBaseHolder() : _eventBase(event_base_new())
                {
                    if (!_eventBase)
                        throw EventBaseHolderException("Failed to create new event_base");
                }

                ~EventBaseHolder()
                {
                    event_base_free(_eventBase);
                }

                event_base* GetBase() const
                {
                    return _eventBase;
                }
            private:
                event_base *_eventBase;

        };

        class HttpEventHolder : public EventBaseHolder
        {
            public:
                typedef std::function<void (char const *, evbuffer *)> _requestHandler;
                HttpEventHolder(SocketHandle sock, _requestHandler const &handler);
            private:
            _requestHandler _handler;
            evhttp *_http;
            static void RawHttpRequestHandler(evhttp_request *request, void *prm)
            {
                reinterpret_cast<HttpEventHolder *>(prm)->ProcessRequest(request);
            }

            void ProcessRequest(evhttp_request *request);
        };

        class ServerThread : private HttpEventHolder, private System::Thread
        {
            public:
                ServerThread(SocketHandle sock, std::string const &rootDir, std::string const &defaultPage)
                    : HttpEventHolder(sock, std::bind(&ServerThread::OnRequest, this, std::placeholders::_1, std::placeholders::_2))
                    , Thread(std::bind(&ServerThread::DispatchProc, this))
                    , _rootDir(rootDir)
                    , _defaultPage(defaultPage)
                    {}
                ~ServerThread()
                {
                    _isRun = false;
                }
            private:
                enum {WaitTimeout = 1000 };
                bool volatile _isRun;
                std::string _rootDir;
                std::string _defaultPage;
                void DispatchProc();
                void OnRequest(char const *resource, evbuffer *outBuffer);
                void GetFullFileName(char const *resource, std::string *fileName) const;
        };

        class HTTPServer : private TCPServerSocket
        {
            public:
                HTTPServer(InetAddress const &locAddr, int backlog, int maxThreadCount, std::string const &rootDir, std::string const &defaultPage);
            private:
                typedef std::shared_ptr<Private::ServerThread> _serverThreadPtr;
                typedef std::vector<_serverThreadPtr> _serverThreadPool;
                _serverThreadPool _serverThreads;
        };

    }
}



#endif // HTTP_SERVER_HPP_INCLUDED
