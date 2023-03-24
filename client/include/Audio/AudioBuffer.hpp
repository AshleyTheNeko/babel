#pragma once

#include "IAudioDevice.hpp"
#include <iostream>
#include <mutex>
#include <portaudio.h>
#include <thread>

namespace babel
{
    class MainWindow;

    class AudioBuffer: public IAudioDevice {
        public:
            AudioBuffer();
            AudioBuffer(AudioBuffer &&) = delete;
            AudioBuffer(const AudioBuffer &) = delete;
            AudioBuffer &operator=(AudioBuffer &&) = delete;
            AudioBuffer &operator=(const AudioBuffer &) = delete;
            ~AudioBuffer();

            void start_call_threads(babel::MainWindow &win, ICodec &codec);
            void set_lock_state(bool state, source_type type);
            bool streaming() const;
            void stop_streaming();
            std::vector<std::vector<float>> &get_frames(source_type type);
            std::vector<std::vector<unsigned char>> &get_bins(source_type type);

        private:
            void init_speaker_thread(ICodec &codec);
            void init_mic_thread(MainWindow &win, ICodec &codec);
            std::vector<std::vector<unsigned char>> mic_bins;
            std::vector<std::vector<float>> mic_frames;
            std::vector<std::vector<unsigned char>> speaker_bins;
            std::vector<std::vector<float>> speaker_frames;
            std::unique_ptr<std::thread> speaker_thread = nullptr;
            std::unique_ptr<std::thread> mic_thread = nullptr;
            bool stream = false;
            std::mutex mic_lock;
            std::mutex speaker_lock;
            PaStream *microphone = nullptr;
            PaStream *speakers = nullptr;
    };
}
