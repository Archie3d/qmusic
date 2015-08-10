#ifndef SERIALIZATIONCONTEXT_H
#define SERIALIZATIONCONTEXT_H

#include <QVariant>
#include "FrameworkApi.h"

class ISerializable;
class ISerializableFactory;

/**
 * @brief The serialization context.
 * Serialization context used for serializing objects with complex dependencies.
 *
 * Serialization context can be furthre encoded as a binary stream (byte array)
 * to be stored in a file or sent over network.
 */
class QMUSIC_FRAMEWORK_API SerializationContext
{
public:

    /**
     * Construct serialization context.
     * @param pFactory Optional pointer to a factory.
     */
    SerializationContext(ISerializableFactory *pFactory = nullptr);

    /**
     * Register a factory.
     * Factories are used only during deserialization.
     * @param pFactory
     */
    void addFactory(ISerializableFactory *pFactory);

    /**
     * Serialize an object.
     * Object is serialized and the handle value is assigned to it.
     * If the same object got serialized within this context the handle will
     * be returned but no duplicate serializetion will be performed.
     * @param pObject Pointer to the object to be serialized.
     * @return Serialized object handle.
     */
    QVariant serialize(ISerializable *pObject);

    /**
     * Deserialize the top-most object.
     * @return Pointer to deserialized object.
     */
    ISerializable* deserialize();

    /**
     * Deserialize an object.
     * @param handle Object handle.
     * @return Pointer to deserialized object.
     */
    ISerializable* deserialize(const QVariant &handle);

    template<typename T> T* deserialize()
    {
        return dynamic_cast<T*>(deserialize());
    }

    template<typename T> T* deserialize(const QVariant &handle)
    {
        return dynamic_cast<T*>(deserialize(handle));
    }

    /**
     * Convert this serialization context to byte array.
     * @return
     */
    QByteArray toByteArray() const;

    /**
     * Initialize this context from byte array.
     * @param data
     */
    void fromByteArray(const QByteArray &data);

    /**
     * Tells whether an error was detected during serialization or deserialization.
     * @return
     */
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

