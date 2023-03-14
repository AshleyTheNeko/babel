#include "Server.hpp"
#include <iostream>

constexpr int PORT_NBR = 2000;

babel::Server::Server() : endpoint(asio::ip::tcp::v4(), PORT_NBR), acceptor(service, endpoint)
{
    using socket_ptr = std::shared_ptr<asio::ip::tcp::socket>;
    socket_ptr socket = std::make_shared<asio::ip::tcp::socket>(service);
    acceptor.async_accept(*socket,
        [socket](__attribute_maybe_unused__ std::error_code error) { std::cout << "New connection from " << socket->remote_endpoint().address() << std::endl; });
}