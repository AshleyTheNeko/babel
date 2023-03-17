#pragma once

#include <string>

namespace babel
{
    class Error : std::exception {
        public:
            Error(const std::string &msg) noexcept;
            Error(Error &&) noexcept = default;
            Error(const Error &) noexcept = default;
            Error &operator=(Error &&) noexcept = default;
            Error &operator=(const Error &) noexcept = default;
            ~Error() noexcept override = default;

            const char *what() const noexcept final;

        private:
            std::string msg;
    };

    class ErrorAllocFail : public Error {
        public:
            ErrorAllocFail(void) noexcept;
    };
}
