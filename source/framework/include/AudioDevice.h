/*
                          qmusic

    Copyright (C) 2015 Arthur Benilov,
    arthur.benilov@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.
*/

#ifndef AUDIODEVICE_H
#define AUDIODEVICE_H

#include <QList>
#include "FrameworkApi.h"

/**
 * Audio device listener interface.
 *
 * Audio device will notify listeners via this interface.
 * Listeners normally should provide output audio data or consume
 * input audio data.
 *
 * Listeners are normally called from an audio processing thread, which
 * can be different than the main application thread.
 */
class QMUSIC_FRAMEWORK_API IAudioDeviceListener
{
public:

    /**
     * Process audio.
     * This method should either provide autput audio data or consime input data.
     * Depending on what kind of data is provides/expected corresponding buffer pointer
     * will point to the valid data or null.
     *
     * @param pInputBuffer Input buffer containing incoming audio.
     * @param pOutputBuffer Output buffer containing outgoing audio.
     * @param nSamples Number of samples to be processed.
     */
    virtual void processAudio(const float *pInputBuffer,
                              float *pOutputBuffer,
                              long nSamples) = 0;

    virtual ~IAudioDeviceListener() {}
};

// To avoid portaudio includes in header.
typedef void PaStream;

/**
 * @brief Audio device based on portaudio.
 *
 * This is a wrapper class that provides interface to an audio device.
 * The actual implementation is based on portaudio, but no portaudio interfaces
 * are actually exposed by this class.
 */
class QMUSIC_FRAMEWORK_API AudioDevice
{
public:

    /// Device information
    struct Info {
        int index;          ///< Deice index.
        QString name;       ///< Device name.
        QString hostApi;    ///< Device host API.
        int nInputs;        ///< Number of input channels.
        int nOutputs;       ///< Number of output channels.
        double sampleRate;  ///< Sample rate.
    };

    /**
     * Create audio device but do not initialize it.
     */
    AudioDevice();
    ~AudioDevice();

    /**
     * Enumerate available audio devices.
     * @return List of audio devices information structures.
     */
    QList<Info> enumarate() const;

    /**
     * Open audio device.
     * @param index Device index (as set in Info structure).
     * @param nInputs Number of input channels.
     * @param nOutputs Number of output channels.
     * @param sampleRate Sample rate to be used.
     * @param bufferSize Audio stream buffer size.
     * @return true on success.
     */
    bool open(int index,
              int nInputs,
              int nOutputs,
              double sampleRate,
              int bufferSize);

    /**
     * Close this audio device.
     * @return true if closed OK.
     */
    bool close();

    /**
     * Tells whether this device is currently open.
     * @return true if device is open.
     */
    bool isOpen() const;

    /**
     * Start audio stream.
     * @note Device has to be open first.
     * @return true if started OK.
     */
    bool start();

    /**
     * Stop audio stream.
     * @return true if stopped.
     */
    bool stop();

    /**
     * Add a listener. Audio device can have multiple registered listeners.
     * Pointer to the listener is not retained.
     * @param pListener Pointer to a listener to be added.
     */
    void addListener(IAudioDeviceListener *pListener);

    /**
     * Remove the listener.
     * Each listener has to properly remove itself before deletion.
     * @param pListener Pointer to the listener to be removed.
     */
    void removeListener(IAudioDeviceListener *pListener);

    /**
     * Returns info structure of this open device.
     * @return This open device info.
     */
    Info openDeviceInfo() const { return m_openDeviceInfo; }

    /**
     * Perform audio processing.
     * This will delegate the processing to registered listeners.
     * @param pInputBuffer Pointer to input audio buffer.
     * @param pOutputBuffer Pointer to output audio buffer.
     * @param nSamples Number of samples to be processed.
     */
    void processAudio(const float *pInputBuffer,
                      float *pOutputBuffer,
                      long nSamples);

private:

    /**
     * Returns device info by its index.
     * @param index Audio device index.
     * @return Device info structure.
     */
    Info getInfo(int index) const;

    PaStream *m_pStream;                ///< Portaudio stream.
    struct Info m_openDeviceInfo;       ///< Info of currently open device.
    QList<IAudioDeviceListener*> m_listeners;   ///< Registered listeners.
};

#endif // AUDIODEVICE_H

