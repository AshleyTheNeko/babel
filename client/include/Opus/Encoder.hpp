#include <opus.h>
#include <vector>

namespace babel
{
    class Encoder {
        public:
            Encoder() = default;
            Encoder(Encoder &&) = delete;
            Encoder(const Encoder &) = delete;
            Encoder &operator=(Encoder &&) = delete;
            Encoder &operator=(const Encoder &) = delete;
            ~Encoder() = default;

            std::vector<opus_int16> get_bytes(const unsigned char *input, int frame_size);
            std::vector<float> get_floats(const unsigned char *input, int frame_size);
        private:
            OpusEncoder *encoder;
            int channels;
    };
}
