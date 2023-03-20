#pragma once

#include "Error.hpp"
#include <memory>

namespace babel
{
    enum RequestType {
        MIN_REQUEST,
        LOGIN,
        REGISTER,
        GET_USERS,
        CALL_PACKET,
        CALL,
        GET_CALLS,
        IN_CALL,
        ACCEPT_CALL,
        HANGUP,
        MAX_REQUEST,
    };

    constexpr int HEADER_MAX_SIZE = 6;
    class Packet {
        public:
            Packet() = default;
            Packet(Packet &&) = delete;
            Packet(const Packet &) = delete;
            Packet &operator=(Packet &&) = delete;
            Packet &operator=(const Packet &) = delete;
            ~Packet() = default;

            char *get_header() noexcept;
            char *get_body() noexcept;
            void set_body_size_from_header();
            int get_size() noexcept;
            RequestType get_type() noexcept;

        private:
            std::shared_ptr<char *> body = nullptr;
            char header[HEADER_MAX_SIZE] = {0};
            RequestType req_type;
            int body_size = 0;
    };
}