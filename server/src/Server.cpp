#include "Server.hpp"
#include <iostream>

babel::Server::Server(Database &db) : endpoint(asio::ip::tcp::v4(), PORT), acceptor(service, endpoint), db(db) { accept(); }

void babel::Server::accept()
{
    temporary_client = std::make_unique<Client>(service, clients, db);

    acceptor.async_accept(temporary_client->get_socket(), std::bind(&babel::Server::handle_accept, this));
}

void babel::Server::handle_accept()
{
    temporary_client->start();
    clients.emplace_back(std::move(temporary_client));
    accept();
}

void babel::Server::run()
{
    service.run();
    accept();
}