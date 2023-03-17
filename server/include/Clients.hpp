#pragma once

#include "Packet.hpp"
#include "Database.hpp"
#include <asio.hpp>

namespace babel
{
    using socket = asio::ip::tcp::socket;
    class Client : public std::enable_shared_from_this<Client> {
        public:
            Client(asio::io_service &service, std::vector<std::shared_ptr<Client>> &clients, Database &db);
            Client(Client &&) = delete;
            Client(const Client &) = delete;
            Client &operator=(Client &&) = delete;
            Client &operator=(const Client &) = delete;
            ~Client() = default;

            void start();
            socket &get_socket();
            void recieve_header();
            void recieve_body(const asio::error_code &error, std::size_t bytes_transferred);
            void parse_body(const asio::error_code &error, std::size_t bytes_transferred);

            void login(std::tuple<std::string, std::string> props);
            void register_user(std::tuple<std::string, std::string> props);
            void get_users();
            void logout();
            void call_user();
            void accept_call();
            

        private:
            Database &db;
            Packet last_packet;
            std::vector<std::shared_ptr<Client>> &clients;
            std::string username;
            int user_id;
            socket sock;
    };
}
