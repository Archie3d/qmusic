#include "Port.h"

Port::Port(Direction dir, const QString &name, QVariant::Type type)
    : m_direction(dir),
      m_name(name),
      m_dataType(type),
      m_pAudioUnit(nullptr)
{
}
