#include "Error.hpp"

babel::Error::Error(const std::string &msg_) noexcept : msg("Babel error: " + msg_) {}

const char *babel::Error::what() const noexcept { return msg.c_str(); }

babel::ErrorAllocFail::ErrorAllocFail(void) noexcept : Error("Pointer fail.") {}