#ifndef SIGNALCHAINFACTORY_H
#define SIGNALCHAINFACTORY_H

#include <QMap>
#include "ISerializableFactory.h"
#include "FrameworkApi.h"

/**
 * Factory of signal chain items.
 */
class QMUSIC_FRAMEWORK_API SignalChainFactory : public ISerializableFactory
{
public:

    SignalChainFactory();

    // ISerializableFactory inrerface
    ISerializable* createObject(const QString &uid) const override;

private:

    /// Register all creators.
    void registerCreators();

    /// List of creators (mapped to serialization UIDs).
    QMap<QString, Creator> m_creators;
};

#endif // SIGNALCHAINFACTORY_H

