#include "Clients.hpp"
#include <iostream>

babel::Client::Client(asio::io_service &service, std::vector<std::shared_ptr<Client>> &clients, Database &db)
    : clients(clients), sock(service), db(db), user_id(0)
{
}

babel::socket &babel::Client::get_socket() { return (sock); }

void babel::Client::start()
{
    clients.push_back(shared_from_this());

    recieve_header();
}

void babel::Client::recieve_header()
{
    async_read(sock, asio::buffer(last_packet.get_header(), babel::HEADER_MAX_SIZE),
        [this](const asio::error_code &error, std::size_t bytes_transferred) { recieve_body(error, bytes_transferred); });
}

void babel::Client::recieve_body(
    __attribute_maybe_unused__ const asio::error_code &error, __attribute_maybe_unused__ std::size_t bytes_transferred)
{
    try {
        last_packet.set_body_size_from_header();
    } catch (const Error &e) {
        std::cerr << e.what() << '\n';
        recieve_header();
        return;
    }
    async_read(sock, asio::buffer(last_packet.get_body(), last_packet.get_size()),
        [this](const asio::error_code &error, std::size_t bytes_transferred) { parse_body(error, bytes_transferred); });
}

static std::tuple<std::string, std::string> get_parameters(std::string &&response)
{
    std::string user;
    std::string pass;
    size_t pos = 0;

    if ((pos = response.find(';')) != std::string::npos) {
        user = response.substr(0, pos);
        pass = response.substr(pos + 1);
        if (pass.empty() || user.empty()) {
            throw babel::Error("Invalid Request");
        }
    } else {
        throw babel::Error("Invalid Request");
    }
    return {user, pass};
}

void babel::Client::parse_body(
    __attribute_maybe_unused__ const asio::error_code &error, __attribute_maybe_unused__ std::size_t bytes_transferred)
{
    for (int i = 0; i < last_packet.get_size(); i++) {
        if (last_packet.get_body()[i] == '\r' || last_packet.get_body()[i] == '\n') {
            last_packet.get_body()[i] = '\0';
        }
    }
    std::cout << sock.remote_endpoint().address().to_string() << " asked for a type " << last_packet.get_type() << " size "
              << last_packet.get_size() << " packet, saying \"" << last_packet.get_body() << "\"." << std::endl;
    try {
        switch (last_packet.get_type()) {
            case LOGIN:
                login(get_parameters(last_packet.get_body()));
                break;
            case REGISTER:
                register_user(get_parameters(last_packet.get_body()));
                break;
            case CALL:
                call_user();
                break;
            case ACCEPT_CALL:
                accept_call();
                break;
            case GET_USERS:
                get_users();
                break;
            default:
                break;
        }
    } catch (const babel::Error &e) {
        std::cerr << e.what() << '\n';
        recieve_header();
    }
}

void babel::Client::login(std::tuple<std::string, std::string> props)
{
    std::string request("SELECT username, password FROM users WHERE username=\"");
    request += (std::get<0>(props) + "\" AND password=\"" + std::get<1>(props) + "\"");
    auto db_ret = db.run_query(std::move(request));

    if (!db_ret.empty()) {
        username = std::get<0>(props);
    }
    async_write(sock, asio::buffer(db_ret.empty() ? "KO" : "OK", 2),
        [this](__attribute_maybe_unused__ const asio::error_code &error, __attribute_maybe_unused__ std::size_t bytes_transferred) {
            recieve_header();
        });
}

void babel::Client::register_user(std::tuple<std::string, std::string> props)
{
    std::string request("INSERT INTO users (username, password) VALUES (\"");
    request += (std::get<0>(props) + "\", \"" + std::get<1>(props) + "\")");
    auto db_ret = db.run_query(std::move(request));

    async_write(sock, asio::buffer(db_ret.empty() ? "KO" : "OK", 2),
        [this](__attribute_maybe_unused__ const asio::error_code &error, __attribute_maybe_unused__ std::size_t bytes_transferred) {
            recieve_header();
        });
}

void babel::Client::get_users()
{
    std::string request("SELECT username, id FROM users");
    auto db_ret = db.run_query(std::move(request));
    std::string ret_str;

    for (auto col : db_ret) {
        ret_str += col["username"] + ";";
    }
    async_write(sock, asio::buffer(ret_str.c_str(), ret_str.size()),
        [this](__attribute_maybe_unused__ const asio::error_code &error, __attribute_maybe_unused__ std::size_t bytes_transferred) {
            recieve_header();
        });
}

void babel::Client::logout() { recieve_header(); }

void babel::Client::call_user() { recieve_header(); }

void babel::Client::accept_call() { recieve_header(); }
