#include <QDebug>
#include "ISerializable.h"
#include "ISerializableFactory.h"
#include "SerializationContext.h"

SerializationContext::SerializationContext(ISerializableFactory *pFactory)
    : m_factories(),
      m_records(),
      m_map(),
      m_error(false)
{
    if (pFactory != nullptr) {
        addFactory(pFactory);
    }
}

void SerializationContext::addFactory(ISerializableFactory *pFactory)
{
    Q_ASSERT(pFactory != nullptr);

    if (!m_factories.contains(pFactory)) {
        m_factories.append(pFactory);
    }
}

QVariant SerializationContext::serialize(ISerializable *pObject)
{
    if (pObject == nullptr || m_error) {
        return QVariant();
    }

    QHash<ISerializable*, int>::iterator it = m_map.find(pObject);

    if (it != m_map.end()) {
        return it.value();  // Return object handle
    }

    // Add object to the records
    int index = m_records.count();
    Record record;
    record.pObject = pObject;
    record.uid = pObject->uid();
    m_records.append(record);
    m_map.insert(pObject, index);

    // Serialize object
    pObject->serialize(m_records[index].data, this);

    return index;
}

ISerializable* SerializationContext::deserialize()
{
    return deserialize(QVariant::fromValue<int>(0));
}

ISerializable* SerializationContext::deserialize(const QVariant &handle)
{
    Q_ASSERT(!m_factories.isEmpty());

    if (m_error || !handle.isValid()) {
        return nullptr;
    }

    int index = handle.toInt();

    Record &record = m_records[index];
    if (record.pObject != nullptr) {
        return record.pObject;
    }

    ISerializable *pObject = nullptr;
    foreach (ISerializableFactory *pFactory, m_factories) {
        pObject = pFactory->createObject(record.uid);
        if (pObject != nullptr) {
            break;
        }
    }

    if (pObject == nullptr) {
        qCritical() << "No factory create object with uid" << record.uid;
    }
    Q_ASSERT(pObject);

    record.pObject = pObject;
    record.uid = pObject->uid();
    m_map.insert(pObject, index);

    pObject->deserialize(record.data, this);

    return pObject;
}

QByteArray SerializationContext::toByteArray() const
{
    QByteArray buffer;
    QDataStream stream(&buffer, QIODevice::WriteOnly);
    stream << *this;
    return qCompress(buffer);
}

void SerializationContext::fromByteArray(const QByteArray &data)
{
    QByteArray buffer = qUncompress(data);
    QDataStream stream(&buffer, QIODevice::ReadOnly);
    stream >> *this;
}

QDataStream& operator << (QDataStream &stream, const SerializationContext &context)
{
    foreach (const SerializationContext::Record &record, context.m_records) {
        stream << record.uid << record.data;
    }

    return stream;
}

QDataStream& operator >> (QDataStream &stream, SerializationContext &context)
{
    while (!stream.atEnd()) {
        SerializationContext::Record record;
        record.pObject = nullptr;
        stream >> record.uid >> record.data;
        context.m_records.append(record);
    }
    return stream;
}
