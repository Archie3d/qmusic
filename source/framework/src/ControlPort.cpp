#include "ControlPort.h"

ControlPort::ControlPort(Direction dir, const QString &name, const QVariant::Type type)
    : Port(dir, name, type)
{
}

void ControlPort::update()
{
}
