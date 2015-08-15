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

#ifndef WAVFILE_H
#define WAVFILE_H

#include <QFile>
#include "FrameworkApi.h"

/**
 * @brief Wav files reader.
 *
 * This class supports only PCM encoded files.
 */
class QMUSIC_FRAMEWORK_API WavFile
{
public:

    /**
     * Wav file chunk IDs.
     */
    enum Chunk {
        Chunk_RiffHeader = 0x46464952,
        Chunk_WavRiff = 0x54651475,
        Chunk_Format = 0x20746D66,
        Chunk_LabeledText = 0x478747C6,
        Chunk_Instrumentation = 0x478747C6,
        Chunk_Sample = 0x6C706D73,
        Chunk_Fact = 0x47361666,
        Chunk_Data = 0x61746164,
        Chunk_Junk = 0x4B4E554A
    };

    /**
     * Data encoding format.
     */
    enum Format {
        Format_PCM = 0x01,
        Format_FloatingPoint = 0x03,
        Format_ALaw = 0x06,
        Format_MuLaw = 0x07,
        Format_IMAADPCM = 0x11,
        Format_YamahaITUG723ADPCM = 0x16,
        Format_GSM610 = 0x31,
        Format_ITUG721ADPCM = 0x40,
        Format_MPEG = 0x50,
        Format_Extensible = 0xFFFE
    };

    /**
     * Initialize the object, but do not open the file.
     * @param filePath Absolute file path.
     */
    WavFile(const QString &filePath);

    /**
     * Destructor.
     * This will also close the file if open.
     */
    ~WavFile();

    /**
     * Open the file for reading.
     * @return true if opened OK.
     */
    bool open();

    /**
     * Close the file.
     */
    void close();

    /**
     * Tells whether the file is open.
     * @return true if the file is open.
     */
    bool isOpen() const;

    /**
     * Tells whether there was an error detected during
     * the data readgin and processing
     * @return true if an error was detected.
     */
    bool isError() const { return m_error; }

    /**
     * Returns the error message.
     * @note The return is valid only if the error flag is set.
     * @return Error message.
     */
    QString errorText() const { return m_errorText; }

    /**
     * Read the file header.
     * This will read the file till the data chunk is encountered.
     * @return true if header has been read successfully.
     */
    bool readHeader();

    /**
     * Read data of a single channel wave file.
     * @param data Read data
     * @return true if read Ok.
     */
    bool readSingleChannelData(QVector<float> &data);

    // format accessors
    Format format() const { return m_format; }
    int numberOfChannels() const { return m_numberOfChannels; }
    int sampleRate() const { return m_sampleRate; }
    int byteRate() const { return m_byteRate; }
    int blockAlign() const { return m_blockAlign; }
    int bitsPerSample() const { return m_bitsPerSample; }
    int numberOfSamples() const { return m_numberOfSamples; }

private:

    void setError(const QString &text);
    void clearError();

    bool readSingleSample(float &v);

    QString m_filePath;
    QFile m_file;
    bool m_error;
    QString m_errorText;

    // Format data
    Format m_format;
    int m_numberOfChannels;
    int m_sampleRate;
    int m_byteRate;
    int m_blockAlign;
    int m_bitsPerSample;
    int m_numberOfSamples;
};

#endif // WAVFILE_H

