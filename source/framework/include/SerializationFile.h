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

#ifndef SERIALIZATIONFILE_H
#define SERIALIZATIONFILE_H

#include <QString>
#include <QByteArray>
#include "FrameworkApi.h"

/**
 * This class handles storing serialized data into a file.
 *
 * A 'magic' number is added to the file header. As well the context
 * if 'signed' with CRC32.
 * Also the API version is stored. It is used to distinguish files crreated
 * by different version of the application.
 */
class QMUSIC_FRAMEWORK_API SerializationFile
{
public:

    /**
     * Initialize serialization file, but do not create/open it.
     * @param path Absolute file path.
     */
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

    QString m_path;         ///< File absolute path.
    quint32 m_magic;        ///< File 'magic' number.
    quint32 m_version;      ///< API version.
    quint32 m_crc;          ///< Content CRC.
    QByteArray m_buffer;    ///< Data buffer (serialization stream).
};

#endif // SERIALIZATIONFILE_H

