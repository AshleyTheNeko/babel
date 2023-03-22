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
            void handle_accept();
            void accept();
        private:
            Database &db;
            std::unique_ptr<Client> temporary_client;
            std::vector<std::unique_ptr<Client>> clients;
            asio::io_service service;
            asio::ip::tcp::endpoint endpoint;
            asio::ip::tcp::acceptor acceptor;
    };
}
