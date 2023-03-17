#include <opus.h>
#include <vector>

namespace babel
{
    class Decoder {
        public:
            Decoder() = default;
            Decoder(Decoder &&) = delete;
            Decoder(const Decoder &) = delete;
            Decoder &operator=(Decoder &&) = delete;
            Decoder &operator=(const Decoder &) = delete;
            ~Decoder() = default;

            std::vector<float> get_floats(const unsigned char *input, int frame_size);
            std::vector<opus_int16> get_bytes(const unsigned char *input, int frame_size);
        private:
            OpusDecoder *decoder;
            int channels;
    };
}
