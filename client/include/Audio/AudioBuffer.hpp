#include <portaudio.h>

namespace babel
{
    class AudioBuffer {
        public:
            AudioBuffer() = default;
            AudioBuffer(AudioBuffer &&) = delete;
            AudioBuffer(const AudioBuffer &) = delete;
            AudioBuffer &operator=(AudioBuffer &&) = delete;
            AudioBuffer &operator=(const AudioBuffer &) = delete;
            ~AudioBuffer() = default;

        private:
            PaStream *stream_;
    };
}
