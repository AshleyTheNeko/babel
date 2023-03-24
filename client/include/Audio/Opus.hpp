#pragma once

#include <opus.h>
#include <vector>
#include "ICodec.hpp"

namespace babel
{
    class Codec: public ICodec {
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