#include "Port.h"

Port::Port(Direction dir, const QString &name, QVariant::Type type)
    : m_direction(dir),
      m_name(name),
      m_dataType(type)
{
}

void Port::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);

    data["name"] = m_name;
}

void Port::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);

    m_name = data["name"].toString();
}
