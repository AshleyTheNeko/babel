#include "Audio/AudioBuffer.hpp"
#include "Error.hpp"
#include "Widgets/MainWindow.hpp"
#include "const_expressions.h"
#include <iostream>

static int mic_callback(const void *input, __attribute_maybe_unused__ void *output, size_t frames,
    __attribute_maybe_unused__ const PaStreamCallbackTimeInfo *time_info, __attribute_maybe_unused__ PaStreamCallbackFlags status,
    void *data)
{
    auto *buffer = static_cast<babel::AudioBuffer *>(data);
    const auto *mic = static_cast<const float *>(input);
    std::vector<float> result;

    if (buffer == nullptr) {
        return (paComplete);
    }
    if (!buffer->streaming()) {
        return (paContinue);
    }
    buffer->set_lock_state(true, source_type::MIC);
    if (input == nullptr) {
        for (size_t i = 0; i < frames; i++) {
            result.push_back(.0F);
            result.push_back(.0F);
        }
    } else {
        for (size_t i = 0; i < frames; i++) {
            result.push_back(*mic);
            mic++;
            result.push_back(*mic);
            mic++;
        }
    }
    buffer->get_frames(source_type::MIC).push_back(result);
    buffer->set_lock_state(false, source_type::MIC);
    return (paContinue);
}

static int speaker_callback(__attribute_maybe_unused__ const void *input, void *output, __attribute_maybe_unused__ size_t frames,
    __attribute_maybe_unused__ const PaStreamCallbackTimeInfo *time_info, __attribute_maybe_unused__ PaStreamCallbackFlags status,
    void *data)
{
    auto *buffer = static_cast<babel::AudioBuffer *>(data);
    auto *speaker = static_cast<float *>(output);
    buffer->set_lock_state(true, source_type::SPEAKER);
    std::vector<std::vector<float>> &bytes = buffer->get_frames(source_type::SPEAKER);
    buffer->set_lock_state(false, source_type::SPEAKER);

    if (buffer == nullptr) {
        return (paComplete);
    }
    if (!buffer->streaming()) {
        return (paContinue);
    }
    if (bytes.empty()) {
        return (paContinue);
    }
    for (const float value : bytes.front()) {
        *speaker = value;
        speaker++;
    }
    buffer->set_lock_state(true, source_type::SPEAKER);
    bytes.erase(bytes.begin());
    buffer->set_lock_state(false, source_type::SPEAKER);
    return (paContinue);
}

babel::AudioBuffer::~AudioBuffer()
{
    if (stream) {
        stream = false;
        mic_thread->join();
        speaker_thread->join();
        Pa_AbortStream(microphone);
        Pa_AbortStream(speakers);
    }
}

babel::AudioBuffer::AudioBuffer()
{
    PaStreamParameters mic_settings;
    PaStreamParameters speaker_settings;
    PaError err = paNoError;

    err = Pa_Initialize();
    if (err != paNoError) {
        throw Error("Microphone failure");
    }
    mic_settings.device = Pa_GetDefaultInputDevice();
    if (mic_settings.device == paNoDevice) {
        throw Error("No default microphone found");
    }
    mic_settings.channelCount = CHANNELS;
    mic_settings.sampleFormat = paFloat32;
    mic_settings.suggestedLatency = Pa_GetDeviceInfo(mic_settings.device)->defaultLowInputLatency;
    mic_settings.hostApiSpecificStreamInfo = nullptr;
    err = Pa_OpenStream(&microphone, &mic_settings, nullptr, RATE, SIZE, paClipOff, mic_callback, this);
    if (err != paNoError) {
        throw Error("Microphone stream failure");
    }

    speaker_settings.device = Pa_GetDefaultOutputDevice();
    if (speaker_settings.device == paNoDevice) {
        throw Error("No speaker found");
    }
    speaker_settings.channelCount = CHANNELS;
    speaker_settings.sampleFormat = paFloat32;
    speaker_settings.suggestedLatency = Pa_GetDeviceInfo(speaker_settings.device)->defaultLowOutputLatency;
    speaker_settings.hostApiSpecificStreamInfo = nullptr;
    err = Pa_OpenStream(&speakers, nullptr, &speaker_settings, RATE, SIZE, paClipOff, speaker_callback, this);
    if (err != paNoError) {
        throw Error("Speaker stream failure");
    }
}

void babel::AudioBuffer::set_lock_state(bool state, source_type type)
{
    if (type == MIC) {
        if (state) {
            mic_lock.lock();
        } else {
            mic_lock.unlock();
        }
    }
    if (type == SPEAKER) {
        if (state) {
            speaker_lock.lock();
        } else {
            speaker_lock.unlock();
        }
    }
}

void babel::AudioBuffer::init_speaker_thread(ICodec &codec)
{
    PaError err = paNoError;

    err = Pa_StartStream(speakers);
    if (err != paNoError) {
        throw Error("Speakers stream start failure");
    }

    speaker_thread = std::make_unique<std::thread>(
        [this](ICodec *codec) {
            std::vector<float> frame_temp;

            try {
                while (stream) {
                    while (!speaker_bins.empty()) {
                        speaker_lock.lock();
                        frame_temp = codec->decode(speaker_bins.front());
                        speaker_bins.erase(speaker_bins.begin());
                        if (frame_temp.empty()) {
                            continue;
                        }
                        speaker_frames.push_back(frame_temp);
                        frame_temp.clear();
                        speaker_lock.unlock();
                    }
                    Pa_Sleep(10);
                }
            } catch (const std::exception &error) {
                std::cerr << error.what() << std::endl;
            }
        },
        &codec);
}

void babel::AudioBuffer::init_mic_thread(MainWindow &win, ICodec &codec)
{
    PaError err = paNoError;

    err = Pa_StartStream(microphone);
    if (err != paNoError) {
        throw Error("Microphone stream start failure");
    }

    mic_thread = std::make_unique<std::thread>(
        [this](babel::MainWindow *win, ICodec *codec) {
            std::vector<unsigned char> bin_temp;
            std::vector<float> frame_temp;

            try {
                while (stream) {
                    do {
                        if (mic_frames.empty() || mic_frames.front().empty()) {
                            break;
                        }
                        mic_lock.lock();
                        frame_temp = mic_frames.front();
                        bin_temp = codec->encode(frame_temp);
                        mic_frames.erase(mic_frames.begin());
                        mic_bins.push_back(bin_temp);
                        mic_lock.unlock();
                        bin_temp.clear();
                        QMetaObject::invokeMethod(win, &MainWindow::send_audio, Qt::QueuedConnection);
                        frame_temp.clear();
                    } while (!frame_temp.empty());
                    Pa_Sleep(10);
                }
            } catch (const std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        },
        &win, &codec);
}

void babel::AudioBuffer::start_streaming(babel::MainWindow &win, ICodec &codec)
{
    if (microphone == nullptr || speakers == nullptr || stream) {
        return;
    }
    stream = true;
    init_speaker_thread(codec);
    init_mic_thread(win, codec);
}

bool babel::AudioBuffer::streaming() const { return stream; }

void babel::AudioBuffer::stop_streaming()
{
    stream = false;
    Pa_AbortStream(microphone);
    Pa_AbortStream(speakers);
}

std::vector<std::vector<float>> &babel::AudioBuffer::get_frames(source_type type)
{
    if (type == MIC) {
        return (mic_frames);
    }
    return (speaker_frames);
}

std::vector<std::vector<unsigned char>> &babel::AudioBuffer::get_bins(source_type type)
{
    if (type == MIC) {
        return (mic_bins);
    }
    return (speaker_bins);
}