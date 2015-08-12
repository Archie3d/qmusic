#include "Port.h"

Port::Port(Direction dir, const QString &name, Signal::Type type)
    : m_direction(dir),
      m_name(name),
      m_dataType(type),
      m_pAudioUnit(nullptr)
{
}
