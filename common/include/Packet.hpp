#pragma once

#include "Error.hpp"
#include <memory>

namespace babel
{
    enum RequestType {
        MIN_REQUEST,
        LOGIN,
        LOGOUT,
        CALL,
        ACCEPT_CALL,
        GET_USERS,
        REGISTER,
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