#include "Clients.hpp"
#include <iostream>

std::string babel::Client::login(std::tuple<std::string, std::string> &&props)
{
    std::string request("SELECT username, password FROM users WHERE username=\"");
    request += (std::get<0>(props) + "\" AND password=\"" + std::get<1>(props) + "\"");
    auto db_ret = db.run_query(std::move(request));

    if (!db_ret.empty()) {
        username = std::get<0>(props);
    }

    return {static_cast<char const *>(db_ret.empty() ? "KO" : "OK")};
}

std::string babel::Client::register_user(std::tuple<std::string, std::string> &&props)
{
    std::string request("INSERT INTO users (username, password) VALUES (\"");

    request += (std::get<0>(props) + "\", \"" + std::get<1>(props) + "\")");
    auto db_ret = db.run_query(std::move(request));

    return {static_cast<char const *>(db_ret.empty() ? "KO" : "OK")};
}

void babel::Client::transfer_packet()
{
    if (call == nullptr) {
        throw Error("Not in a call");
    }
    std::string packet = std::string(last_packet.get_header(), last_packet.get_header() + HEADER_MAX_SIZE) +
        std::string(last_packet.get_body(), last_packet.get_body() + last_packet.get_size());

    async_write(call->get_socket(), asio::buffer(packet.c_str(), packet.size()),
        [this](__attribute_maybe_unused__ const asio::error_code &error, __attribute_maybe_unused__ std::size_t bytes_transferred) {});
    recieve_header();
}

std::string babel::Client::in_call()
{
    if (awaiting_pickup) {
        return ("1");
    }
    if (call != nullptr) {
        return ("2");
    }
    return ("0");
}

std::string babel::Client::get_calls()
{
    std::string resp = (static_cast<char const *>(call_request.empty() ? "0" : "1")) + call_request;

    return (resp);
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

    return (ret_str);
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
            return ("0" + name);
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
            call = client.get();
            client->call = this;
            call->awaiting_pickup = false;
            return ("0" + call->get_name());
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
        call->call = nullptr;
        call = nullptr;
    }

    return ("OK");
}