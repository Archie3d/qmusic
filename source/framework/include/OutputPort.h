#ifndef OUTPUTPORT_H
#define OUTPUTPORT_H

#include <QSharedPointer>
#include "FrameworkApi.h"
#include "IAudioUnit.h"
#include "Port.h"

class QMUSIC_FRAMEWORK_API OutputPort : public Port
{
public:

    const static QString UID;

    OutputPort();
    OutputPort(const QString &name, QVariant::Type type);

    QVariant value() const override { return m_value; }
    void setValue(const QVariant &value);

    void setAudioUnit(IAudioUnit *pAudioUnit);
    IAudioUnit* audioUnit() const { return m_pAudioUnit; }

    void update() override;

    // ISerializable interface
    QString uid() const { return UID; }
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);
    static ISerializable* create() { return new OutputPort(); }

private:

    /// Pointer to the audio unit this port belongs to.
    IAudioUnit *m_pAudioUnit;

    /// Value stored in this output port.
    QVariant m_value;
};

#endif // OUTPUTPORT_H

