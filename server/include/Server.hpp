#pragma once

#include <asio.hpp>

namespace babel
{
    class Server {
        public:
            Server();
            Server(Server &&) = delete;
            Server(const Server &) = delete;
            Server &operator=(Server &&) = delete;
            Server &operator=(const Server &) = delete;
            ~Server() = default;

        private:
            asio::io_service service;
            asio::ip::tcp::endpoint endpoint;
            asio::ip::tcp::acceptor acceptor;
    };
}
