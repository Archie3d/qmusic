#ifndef AUDIOUNITPLUGIN_H
#define AUDIOUNITPLUGIN_H

#include <QObject>
#include <QIcon>
#include "FrameworkApi.h"

class QWidget;
class AudioUnit;

class QMUSIC_FRAMEWORK_API AudioUnitPlugin : public QObject
{
    Q_OBJECT

    friend class AudioUnitsManager;

public:

    const static QString MimeDataFormat;

    AudioUnitPlugin(QObject *pParent = nullptr);

    virtual AudioUnit* createInstance() = 0;
    virtual void initialize() {}
    virtual void cleanup() {}

    QString uid() const { return m_uid; }
    QString name() const { return m_name; }
    QString category() const { return m_category; }
    QString version() const { return m_version; }
    virtual QIcon icon() const { return QIcon(); }

protected:

    void setUid(const QString &uid) { m_uid = uid; }
    void setName(const QString &name) { m_name = name; }
    void setCategory(const QString &cat) { m_category = cat; }
    void setVersion(const QString &v) { m_version = v; }

private:

    QString m_uid;      ///< Plugin unique ID.
    QString m_name;     ///< Audio unit plugin name.
    QString m_category; ///< Audio unit category.
    QString m_version;  ///< Version info.
};

Q_DECLARE_INTERFACE(AudioUnitPlugin, "qmusic.audiounit.plugin")

#endif // AUDIOUNITPLUGIN_H

