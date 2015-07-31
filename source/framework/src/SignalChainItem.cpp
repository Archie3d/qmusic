#include "SerializationContext.h"
#include "SignalChainItem.h"

SignalChainItem::SignalChainItem(Type type, QGraphicsItem *pParent)
    : QGraphicsPathItem(pParent),
      m_type(type)
{
}

void SignalChainItem::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);

    data["position"] = pos();
}

void SignalChainItem::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);

    setPos(data["position"].toPointF());
}
