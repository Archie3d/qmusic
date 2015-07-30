#ifndef SERIALIZATIONFILE_H
#define SERIALIZATIONFILE_H

#include <QString>
#include <QByteArray>
#include "FrameworkApi.h"

class QMUSIC_FRAMEWORK_API SerializationFile
{
public:

    SerializationFile(const QString &path);

    quint32 magic() const { return m_magic; }
    void setMagic(quint32 v) { m_magic = v; }

    quint32 version() const { return m_version; }
    void setVersion(quint32 v) { m_version= v; }

    quint32 crc() const { return m_crc; }

    const QByteArray& buffer() const { return m_buffer; }
    void setBuffer(const QByteArray &buffer);

    bool load();
    bool save() const;

    static quint32 crc32(const char *pBuffer, int size);
    static quint32 packVersionString(const QString &v);
    QString unpackVersionString(quint32 v);

private:

    QString m_path;
    quint32 m_magic;
    quint32 m_version;
    quint32 m_crc;
    QByteArray m_buffer;
};

#endif // SERIALIZATIONFILE_H

