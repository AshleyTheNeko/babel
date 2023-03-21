#include "Audio/Opus.hpp"
#include "Error.hpp"

babel::Codec::Codec()
{
    int err = 0;

    decoder = opus_decoder_create(RATE, CHANNELS, &err);
    if (err < 0) {
        throw Error("Decoder failure: " + std::string(opus_strerror(err)));
    }
    encoder = opus_encoder_create(RATE, CHANNELS, OPUS_APPLICATION_VOIP, &err);
    if (err < 0) {
        throw Error("Encoder failure: " + std::string(opus_strerror(err)));
    }
}

babel::Codec::~Codec()
{
    if (decoder != nullptr) {
        opus_decoder_destroy(decoder);
    }
    if (encoder != nullptr) {
        opus_encoder_destroy(encoder);
    }
}

std::vector<unsigned char> babel::Codec::encode(const std::vector<float> &values)
{
    int res_size = 0;
    unsigned char res[MAX_SIZE];

    if (encoder == nullptr) {
        throw Error("Encoder not available.");
    }
    if (values.size() != SIZE * CHANNELS || values.empty()) {
        return {};
    }
    res_size = opus_encode_float(encoder, &values[0], SIZE, static_cast<unsigned char *>(res), MAX_SIZE);
    return {static_cast<unsigned char *>(res), static_cast<unsigned char *>(res) + res_size};
}

std::vector<float> babel::Codec::decode(const std::vector<unsigned char> &values)
{
    std::vector<float> res;
    int err = 0;
    float bytes[SIZE * CHANNELS];

    if (decoder == nullptr) {
        throw Error("Decoder not available.");
    }
    if (values.empty()) {
        return (res);
    }
    err = opus_decode_float(decoder, &values[0], static_cast<opus_int32>(values.size()), static_cast<float *>(bytes), SIZE, 0);
    if (err < 0) {
        throw Error("Decoder error");
    }
    for (float byte : bytes) {
        res.push_back(byte);
    }
    return (res);
}