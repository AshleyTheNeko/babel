#include "Clients.hpp"
#include <iostream>

babel::Client::Client(asio::io_service &service, std::vector<std::shared_ptr<Client>> &clients, Database &db)
    : clients(clients), sock(service), db(db), user_id(0)
{
}

babel::Client::~Client()
{
    if (call != nullptr)
        call.reset();
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

    std::cout << sock.remote_endpoint().address().to_string() << " asked for a type " << last_packet.get_type() << " size "
              << last_packet.get_size() << " packet, saying \"" << last_packet.get_body() << "\"." << std::endl;
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
                response = transfer_packet();
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

std::string babel::Client::login(std::tuple<std::string, std::string> &&props)
{
    std::string request("SELECT username, password FROM users WHERE username=\"");
    request += (std::get<0>(props) + "\" AND password=\"" + std::get<1>(props) + "\"");
    auto db_ret = db.run_query(std::move(request));

    if (!db_ret.empty()) {
        username = std::get<0>(props);
    }

    return (std::to_string(RequestType::LOGIN) + "/" + std::string(static_cast<char const *>(db_ret.empty() ? "KO" : "OK")));
}

std::string babel::Client::register_user(std::tuple<std::string, std::string> &&props)
{
    std::string request("INSERT INTO users (username, password) VALUES (\"");

    request += (std::get<0>(props) + "\", \"" + std::get<1>(props) + "\")");
    auto db_ret = db.run_query(std::move(request));

    return (std::to_string(RequestType::LOGIN) + "/" + std::string(static_cast<char const *>(db_ret.empty() ? "KO" : "OK")));
}

std::string babel::Client::transfer_packet() {}

std::string babel::Client::in_call()
{
    if (awaiting_pickup) {
        return (std::to_string(RequestType::IN_CALL) + "/1");
    }
    if (call != nullptr) {
        return (std::to_string(RequestType::IN_CALL) + "/2");
    }
    return (std::to_string(RequestType::IN_CALL) + "/0");
}

std::string babel::Client::get_calls()
{
    std::string resp = (static_cast<char const *>(call_request.empty() ? "0" : "1")) + call_request;

    return (std::to_string(RequestType::GET_CALLS) + "/" + resp);
}

std::string babel::Client::get_users()
{
    std::string ret_str = ";";

    for (auto const &client : clients) {
        if (client->get_name() == username) {
            continue;
        }
        ret_str += client->get_name() + ";";
    }

    return (std::to_string(RequestType::GET_USERS) + "/" + ret_str);
}

std::string babel::Client::call_user(std::string &&name)
{
    if (!call_request.empty()) {
        throw Error("Already requested");
    }

    if (username.empty()) {
        throw Error("Not logged");
    }

    for (auto const &client : clients) {
        if (client->get_name() == name) {
            if (!client->call_request.empty() || client->call != nullptr) {
                throw Error("User already in call");
            }
            client->call_request = this->username;
            awaiting_pickup = true;
            return (std::to_string(RequestType::CALL) + "/0" + name);
        }
    }

    throw Error("No such client");
}

std::string babel::Client::accept_call()
{
    if (call_request.empty()) {
        throw Error("No call request");
    }

    for (auto const &client : clients) {
        if (client->get_name() == call_request) {
            call_request.clear();
            call = client;
            client->call = shared_from_this();
            call->awaiting_pickup = false;
            return (std::to_string(RequestType::ACCEPT_CALL) + "/0" + call->get_name());
        }
    }

    throw Error("No such client");
}

std::string babel::Client::hangup()
{
    if (call == nullptr && call_request.empty()) {
        throw Error("Not in a call");
    }

    if (!call_request.empty()) {
        for (auto const &client : clients) {
            if (client->get_name() == call_request) {
                client->awaiting_pickup = false;
            }
        }
        call_request.clear();
    }
    if (call != nullptr) {
        call->call.reset();
        call.reset();
    }

    return (std::to_string(RequestType::HANGUP) + "/" + "OK");
}