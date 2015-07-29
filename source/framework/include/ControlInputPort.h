#ifndef CONTROLINPUTPORT_H
#define CONTROLINPUTPORT_H

#include <QSharedPointer>
#include "FrameworkApi.h"
#include "IAudioUnit.h"
#include "ControlPort.h"

class QMUSIC_FRAMEWORK_API ControlInputPort : public ControlPort
{
public:

    ControlInputPort(IAudioUnit *pAudioUnit, const QString &name, QVariant::Type type);

    void setValue(const QVariant &value);
    QVariant value() const { return m_value; }

    void setAudioUnit(IAudioUnit *pAudioUnit);

private:

    IAudioUnit *m_pAudioUnit;
    QVariant m_value;   ///< Saved input value.
};

typedef QSharedPointer<ControlInputPort> ControlInputPortPtr;

#endif // CONTROLINPUTPORT_H

