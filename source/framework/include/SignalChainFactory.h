#ifndef SIGNALCHAINFACTORY_H
#define SIGNALCHAINFACTORY_H

#include <QMap>
#include "ISerializableFactory.h"
#include "FrameworkApi.h"

class QMUSIC_FRAMEWORK_API SignalChainFactory : public ISerializableFactory
{
public:

    SignalChainFactory();

    // ISerializableFactory inrerface
    ISerializable* createObject(const QString &uid) const override;

private:

    void registerCreators();

    QMap<QString, Creator> m_creators;
};

#endif // SIGNALCHAINFACTORY_H

