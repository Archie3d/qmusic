#ifndef SIGNALCHAINITEM_H
#define SIGNALCHAINITEM_H

#include <QPen>
#include <QGraphicsPathItem>
#include "FrameworkApi.h"

class QMUSIC_FRAMEWORK_API SignalChainItem : public QGraphicsPathItem
{
public:

    enum Type {
        Type_First = QGraphicsItem::UserType + 1,
        Type_Invalid = Type_First,
        Type_InputPort = Type_First + 1,
        Type_OutputPort = Type_First + 2,
        Type_Connection = Type_First + 3,
        Type_AudioUnit = Type_First + 4
    };

    SignalChainItem(Type type = Type_Invalid, QGraphicsItem *pParent = nullptr);

    int type() const override { return m_type; }

private:

    /// Graphics node type;
    Type m_type;
};

#endif // SIGNALCHAINITEM_H

