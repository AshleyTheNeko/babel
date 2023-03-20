#pragma once

#include "Database.hpp"
#include "Clients.hpp"
#include <asio.hpp>
#include <vector>

namespace babel
{
    constexpr int PORT = 2000;
    class Server {
        public:
            Server(Database &db);
            Server(Server &&) = delete;
            Server(const Server &) = delete;
            Server &operator=(Server &&) = delete;
            Server &operator=(const Server &) = delete;
            ~Server() = default;

            void run();
            void handle_accept(const std::shared_ptr<Client> &client);
            void accept();
        private:
            Database &db;
            std::vector<std::shared_ptr<Client>> clients;
            asio::io_service service;
            asio::ip::tcp::endpoint endpoint;
            asio::ip::tcp::acceptor acceptor;
    };
}
