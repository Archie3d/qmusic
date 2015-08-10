#ifndef AUDIOUNITPLUGIN_H
#define AUDIOUNITPLUGIN_H

#include <QObject>
#include <QIcon>
#include "FrameworkApi.h"

class QWidget;
class AudioUnit;

/**
 * @brief Audio unit plugin.
 *
 * Audio unit are instantiated from Qt-style plugins.
 * Every audio unit plugin must be inherited from this class.
 *
 * Each audio unit plugin is identified by its UID string.
 * Random MD5 hash is used as a UID. Also each plugin defines its
 * category (used to group audio units into categories lists), name,
 * and version.
 *
 * Optionally a plugin may define an icon that will be using in the
 * audio units library and on signal chain canvas.
 */
class QMUSIC_FRAMEWORK_API AudioUnitPlugin : public QObject
{
    Q_OBJECT

    friend class AudioUnitsManager;

public:

    // Mime data used for drag-and-drop operations.
    const static QString MimeDataFormat;

    /// Constructor.
    AudioUnitPlugin(QObject *pParent = nullptr);

    /**
     * Create an audio unit instance.
     * @return Pointer to a new instance.
     */
    virtual AudioUnit* createInstance() = 0;

    /**
     * Initialize plugin.
     * This method is called by plugins manager.
     */
    virtual void initialize() {}

    /**
     * Clean-up plugin.
     * This method is called by plugins manager upon unloading.
     */
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

