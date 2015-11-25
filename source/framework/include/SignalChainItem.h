/*
                          qmusic

    Copyright (C) 2015 Arthur Benilov,
    arthur.benilov@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.
*/

#ifndef SIGNALCHAINITEM_H
#define SIGNALCHAINITEM_H

#include <QPen>
#include <QGraphicsPathItem>
#include "ISerializable.h"
#include "FrameworkApi.h"

/**
 * @brief Abstract base class for signal chain graphics items.
 */
class QMUSIC_FRAMEWORK_API SignalChainItem : public QGraphicsPathItem
{
public:

    /// Item tyype.
    enum Type {
        Type_First = QGraphicsItem::UserType + 1,
        Type_Invalid = Type_First,          ///< Invalid item (not used).
        Type_InputPort = Type_First + 1,    ///< Input port item.
        Type_OutputPort = Type_First + 2,   ///< Output port item.
        Type_Connection = Type_First + 3,   ///< Connection path item.
        Type_AudioUnit = Type_First + 4     ///< Audio unit item.
    };

    /**
     * Construct a signal chain item.
     * @param type Item type.
     * @param pParent Pointer to parent item.
     */
    SignalChainItem(Type type = Type_Invalid, QGraphicsItem *pParent = nullptr);

    /**
     * Returns signal chain item type.
     * @return
     */
    int type() const override { return m_type; }

private:

    /// Graphics node type;
    Type m_type;
};

#endif // SIGNALCHAINITEM_H

