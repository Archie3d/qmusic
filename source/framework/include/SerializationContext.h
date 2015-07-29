#ifndef SERIALIZATIONCONTEXT_H
#define SERIALIZATIONCONTEXT_H

#include <QVariant>
#include "FrameworkApi.h"

class ISerializable;
class ISerializableFactory;

/**
 * @brief The serialization context.
 * Serialization context used for serializing objects with complex dependencies.
 */
class QMUSIC_FRAMEWORK_API SerializationContext
{
public:

    SerializationContext(ISerializableFactory *pFactory = nullptr);

    void addFactory(ISerializableFactory *pFactory);
    QVariant serialize(ISerializable *pObject);
    ISerializable* deserialize();
    ISerializable* deserialize(const QVariant &handle);
    QByteArray toByteArray() const;
    void fromByteArray(const QByteArray &data);
    bool isError() const { return m_error; }

    // Stream I/O
    QMUSIC_FRAMEWORK_API friend QDataStream& operator << (QDataStream &stream, const SerializationContext &context);
    QMUSIC_FRAMEWORK_API friend QDataStream& operator >> (QDataStream &stream, SerializationContext &context);

protected:

    /**
     * @brief Serializable object data.
     */
    struct Record {
        ISerializable *pObject;
        QString uid;
        QVariantMap data;
    };

    QList<ISerializableFactory*> m_factories;
    QList<Record> m_records;
    QHash<ISerializable*, int> m_map;

private:

    Q_DISABLE_COPY(SerializationContext)

    bool m_error;
};

#endif // SERIALIZATIONCONTEXT_H

