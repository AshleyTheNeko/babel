#include "Clients.hpp"
#include <iostream>

babel::Client::Client(asio::io_service &service, std::vector<std::shared_ptr<Client>> &clients, Database &db)
    : clients(clients), sock(service), db(db), user_id(0)
{
}

babel::Client::~Client()
{
    if (call != nullptr) {
        call.reset();
    }
}

babel::socket &babel::Client::get_socket() { return (sock); }

void babel::Client::start()
{
    clients.push_back(shared_from_this());

    recieve_header();
}

bool babel::Client::is_delete() const { return to_delete; }

std::string &babel::Client::get_name() { return (username); }

void babel::Client::recieve_header()
{
    async_read(sock, asio::buffer(last_packet.get_header(), babel::HEADER_MAX_SIZE),
        [this](const asio::error_code &error, std::size_t bytes_transferred) { recieve_body(error, bytes_transferred); });
}

void babel::Client::recieve_body(const asio::error_code &error, __attribute_maybe_unused__ std::size_t bytes_transferred)
{
    if (error == asio::error::eof) {
        to_delete = true;
        clients.erase(
            std::remove_if(clients.begin(), clients.end(), [this](const std::shared_ptr<Client> &obj) { return obj->is_delete(); }),
            clients.end());
        return;
    }
    // std::cout << std::string(last_packet.get_header(), last_packet.get_header() + HEADER_MAX_SIZE) << std::endl;
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
    if (error == asio::error::eof) {
        to_delete = true;
        clients.erase(
            std::remove_if(clients.begin(), clients.end(), [this](const std::shared_ptr<Client> &obj) { return obj->is_delete(); }),
            clients.end());
        return;
    }

    for (int i = 0; i < last_packet.get_size(); i++) {
        if (last_packet.get_body()[i] == '\r' || last_packet.get_body()[i] == '\n') {
            last_packet.get_body()[i] = '\0';
        }
    }

    // std::cout << sock.remote_endpoint().address().to_string() << " asked for a type " << last_packet.get_type() << " size "
    //           << last_packet.get_size() << " packet, " << bytes_transferred << "bytes were transf, saying \"" << last_packet.get_body()
    //           << "\"." << std::endl;
    std::string response;

    try {
        switch (last_packet.get_type()) {
            case LOGIN:
                response = login(get_parameters(last_packet.get_body()));
                break;
            case REGISTER:
                response = register_user(get_parameters(last_packet.get_body()));
                break;
            case CALL_PACKET:
                transfer_packet();
                break;
            case IN_CALL:
                response = in_call();
                break;
            case CALL:
                response = call_user(last_packet.get_body());
                break;
            case ACCEPT_CALL:
                response = accept_call();
                break;
            case GET_USERS:
                response = get_users();
                break;
            case HANGUP:
                response = hangup();
                break;
            case GET_CALLS:
                response = get_calls();
                break;
            default:
                break;
        }
    } catch (const babel::Error &e) {
        std::cerr << e.what() << '\n';
        std::string err = std::to_string(last_packet.get_type()) + "/1" + e.what();
        async_write(sock, asio::buffer(err.c_str(), err.size()),
            [this](const asio::error_code &error, __attribute_maybe_unused__ std::size_t bytes_transferred) {
                if (error == asio::error::eof) {
                    to_delete = true;
                    clients.erase(std::remove_if(clients.begin(), clients.end(),
                                      [this](const std::shared_ptr<Client> &obj) { return obj->is_delete(); }),
                        clients.end());
                    return;
                }
                recieve_header();
            });
        return;
    }

    if (!response.empty()) {
        async_write(sock, asio::buffer(response.c_str(), response.size()),
            [this](const asio::error_code &error, __attribute_maybe_unused__ std::size_t bytes_transferred) {
                if (error == asio::error::eof) {
                    to_delete = true;
                    clients.erase(std::remove_if(clients.begin(), clients.end(),
                                      [this](const std::shared_ptr<Client> &obj) { return obj->is_delete(); }),
                        clients.end());
                    return;
                }
                recieve_header();
            });
    }
}
