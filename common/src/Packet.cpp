#include "Packet.hpp"
#include <string>

char *babel::Packet::get_body() noexcept { return (*body); };

char *babel::Packet::get_header() noexcept { return (static_cast<char *>(header)); }

void babel::Packet::set_body_size_from_header()
{
    body_size = static_cast<int>(std::strtol(static_cast<char *>(header), nullptr, 10));
    req_type = static_cast<RequestType>(std::strtol(static_cast<char *>(header) + 5, nullptr, 10));
    if (body_size < 1) {
        throw Error("Invalid size");
    }
    if (req_type <= MIN_REQUEST || req_type >= MAX_REQUEST) {
        throw Error("Invalid type");
    }
    try {
        body = std::make_shared<char *>(new char[body_size + 1]);
    } catch (const std::exception &e) {
        body = nullptr;
        throw ErrorAllocFail();
    }
    std::fill_n(*body, body_size + 1, 0);
}

int babel::Packet::get_size() noexcept { return (body_size); }

babel::RequestType babel::Packet::get_type() noexcept { return (req_type); }