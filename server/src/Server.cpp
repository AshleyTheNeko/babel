#include "Server.hpp"
#include <iostream>

constexpr int PORT_NBR = 2000;

babel::Server::Server(Database &db) : endpoint(asio::ip::tcp::v4(), PORT_NBR), acceptor(service, endpoint), db(db) { accept(); }

void babel::Server::accept()
{
    std::shared_ptr<Client> cli = std::make_shared<Client>(service, clients, db);

    acceptor.async_accept(cli->get_socket(), std::bind(&babel::Server::handle_accept, this, cli));
}

void babel::Server::handle_accept(const std::shared_ptr<Client> &client)
{
    client->start();
    accept();
}

void babel::Server::run()
{
    service.run();
    accept();
}