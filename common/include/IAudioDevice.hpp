#pragma once

#include "ICodec.hpp"

enum source_type {
    MIC,
    SPEAKER,
};

namespace babel {
    class MainWindow;
}

class IAudioDevice {
    public:
        IAudioDevice() = default;
        IAudioDevice(IAudioDevice &&) = delete;
        IAudioDevice(const IAudioDevice &) = delete;
        IAudioDevice &operator=(IAudioDevice &&) = delete;
        IAudioDevice &operator=(const IAudioDevice &) = delete;
        virtual ~IAudioDevice() = default;

        virtual void start_call_threads(babel::MainWindow &win, ICodec &codec) = 0;
        virtual void set_lock_state(bool state, source_type type) = 0;
        virtual bool streaming() const = 0;
        virtual void stop_streaming() = 0;
        virtual std::vector<std::vector<float>> &get_frames(source_type type) = 0;
        virtual std::vector<std::vector<unsigned char>> &get_bins(source_type type) = 0;
};