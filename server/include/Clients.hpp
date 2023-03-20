#pragma once

#include "Database.hpp"
#include "Packet.hpp"
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
            ~Client();

            void start();
            bool is_delete() const;
            socket &get_socket();
            void recieve_header();
            void recieve_body(const asio::error_code &error, std::size_t bytes_transferred);
            void parse_body(const asio::error_code &error, std::size_t bytes_transferred);

            std::string login(std::tuple<std::string, std::string> &&props);
            std::string register_user(std::tuple<std::string, std::string> &&props);
            std::string get_users();
            std::string get_calls();
            std::string in_call();
            std::string transfer_packet();
            std::string call_user(std::string &&username);
            std::string accept_call();
            std::string hangup();
            std::string &get_name();

        private:
            bool to_delete = false;
            bool awaiting_pickup = false;
            Database &db;
            Packet last_packet;
            std::vector<std::shared_ptr<Client>> &clients;
            std::string username;
            std::string call_request;
            std::shared_ptr<Client> call;
            int user_id;
            socket sock;
    };
}
