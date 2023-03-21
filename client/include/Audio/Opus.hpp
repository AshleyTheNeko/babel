#pragma once

#include <opus.h>
#include <vector>

namespace babel
{
    constexpr std::size_t RATE = 48000;
    constexpr std::size_t SIZE = 960;
    constexpr std::size_t CHANNELS = 2;
    constexpr std::size_t MAX_SIZE = 2048;
    class Codec {
        public:
            Codec();
            Codec(Codec &&) = delete;
            Codec(const Codec &) = delete;
            Codec &operator=(Codec &&) = delete;
            Codec &operator=(const Codec &) = delete;
            ~Codec();

            std::vector<unsigned char> encode(const std::vector<float> &values);
            std::vector<float> decode(const std::vector<unsigned char> &values);

        private:
            OpusDecoder *decoder;
            OpusEncoder *encoder;
    };
}