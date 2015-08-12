#include <QDebug>
#include "WavFile.h"

#define WAVE_SIGNATURE (0x45564157)

template<typename T>
bool read(QFile &file, T &v)
{
    if (file.isOpen()) {
        T data;
        qint64 r = file.read(reinterpret_cast<char*>(&data), sizeof(data));
        if (r == sizeof(data)) {
            v = data;
            return true;
        } else {
            qCritical() << "Expected" << sizeof(data) << "read" << r;
        }
    }
    return false;
}

WavFile::WavFile(const QString &filePath)
    : m_filePath(filePath)
{
    clearError();
}

WavFile::~WavFile()
{
    m_file.close();
}

bool WavFile::open()
{
    clearError();
    if (m_file.isOpen()) {
        setError("File is already open");
        return false;
    }

    m_file.setFileName(m_filePath);
    return m_file.open(QIODevice::ReadOnly);
}

void WavFile::close()
{
    m_file.close();
    clearError();
}

bool WavFile::isOpen() const
{
    return m_file.isOpen();
}

bool WavFile::readHeader()
{
    clearError();
    if (!isOpen()) {
        setError("File is not open");
        return false;
    }

    // Move to the beginning of the file
    m_file.seek(0);

    quint32 chunkId = 0;
    quint32 chunkSize = 0;
    bool isData = false;

    while (!isData && !isError()) {
        if (!read<quint32>(m_file, chunkId)) {
            setError("Unable to read the file chunk ID");
            break;
        }
        if (!read<quint32>(m_file, chunkSize)) {
            setError("Unable to read chunk size");
            break;
        }

        switch (static_cast<Chunk>(chunkId)) {
        case Chunk_RiffHeader: {
            quint32 wave = 0;
            bool ok = read<quint32>(m_file, wave);
            if (!ok) {
                setError("Unable to read WAVE signature");
            } else if (wave != WAVE_SIGNATURE) {
                setError(QString("WAVE signature not found, but 0x%1 instead")
                         .arg(QString::number(wave, 16)));
            }
            break;
        }
        case Chunk_Format: {
            quint16 fmt;
            quint16 channels;
            quint32 sampleRate;
            quint32 byteRate;
            quint16 blockAlign;
            quint16 bitsPerSample;

            bool ok = true;
            ok &= read<quint16>(m_file, fmt);
            ok &= read<quint16>(m_file, channels);
            ok &= read<quint32>(m_file, sampleRate);
            ok &= read<quint32>(m_file, byteRate);
            ok &= read<quint16>(m_file, blockAlign);
            ok &= read<quint16>(m_file, bitsPerSample);

            if (ok) {
                m_format = static_cast<Format>(fmt);
                m_numberOfChannels = static_cast<int>(channels);
                m_sampleRate = static_cast<int>(sampleRate);
                m_byteRate = static_cast<int>(byteRate);
                m_blockAlign = static_cast<int>(blockAlign);
                m_bitsPerSample = static_cast<int>(bitsPerSample);
            } else {
                setError("Unable to read format data");
            }

            if (m_blockAlign != m_numberOfChannels * m_bitsPerSample / 8) {
                setError("Invalid block align");
            }

            break;
        }
        case Chunk_Data:
            isData = true;
            m_numberOfSamples = chunkSize / m_blockAlign;
            break;
        default:
            // Skip unknown chunk
            m_file.seek(m_file.pos() + chunkSize);
            break;
        }
    }

    return !isError();
}

bool WavFile::readSingleChannelData(QVector<float> &data)
{
    clearError();
    if (!isOpen()) {
        setError("File is not open");
        return false;
    }
    if (m_format != Format_PCM) {
        setError("unsupported format (PCM expected)");
        return false;
    }
    if (m_numberOfChannels != 1) {
        setError(QString("Single channel expected, but %1 found").arg(m_numberOfChannels));
        return false;
    }

    bool ok = true;
    int samples = 0;
    while (ok && samples < m_numberOfSamples && !m_file.atEnd()) {
        float v;
        ok &= readSingleSample(v);
        if (ok) {
            data.append(v);
            samples++;
        }
    }

    if (!ok) {
        setError("Unable to read samples data");
    }

    return ok;
}

void WavFile::setError(const QString &text)
{
    m_error = true;
    m_errorText = text;
}

void WavFile::clearError()
{
    m_error = false;
    m_errorText.clear();
}

bool WavFile::readSingleSample(float &v)
{
    bool ok = true;
    switch (m_bitsPerSample) {
    case 8: {
        quint8 data;
        ok &= read<quint8>(m_file, data);
        if (ok) {
            v = float(data - 127) / 128.0f;
        }
        break;
    }
    case 16: {
        qint16 data;
        ok &= read<qint16>(m_file, data);
        if (ok) {
            v = float(data) / 32768.0f;
        }
        break;
    }
    case 24: {
        char buf[3];
        qint64 r = m_file.read(buf, 3);
        ok &= r == 3;
        if (ok) {
            qint32 value = qint32(buf[0]) << 24;
            value |= qint32(buf[1]) << 16;
            value |= qint32(buf[2]) << 8;
            v = float(value) / 2147483648.0f;
        }
        break;
    }
    case 32: {
        qint32 data;
        ok &= read<qint32>(m_file, data);
        if (ok) {
            v = float(data) / 2147483648.0f;
        }
        break;
    }
    default:
        ok = false;
        break;
    }

    return ok;
}
