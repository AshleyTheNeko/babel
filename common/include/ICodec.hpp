#pragma once

#include <vector>

class ICodec {
    public:
        ICodec() = default;
        ICodec(ICodec &&) = delete;
        ICodec(const ICodec &) = delete;
        ICodec &operator=(ICodec &&) = delete;
        ICodec &operator=(const ICodec &) = delete;
        virtual ~ICodec() = default;

        virtual std::vector<unsigned char> encode(const std::vector<float> &values) = 0;
        virtual std::vector<float> decode(const std::vector<unsigned char> &values) = 0;
};