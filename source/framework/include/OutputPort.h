#ifndef OUTPUTPORT_H
#define OUTPUTPORT_H

#include <QSharedPointer>
#include "FrameworkApi.h"
#include "IAudioUnit.h"
#include "Port.h"

/**
 * @brief Output port.
 *
 * Outgoing signal port.
 * Output port keeps the value stored by its audio unit.
 * This value is then supplied to all connected input ports.
 */
class QMUSIC_FRAMEWORK_API OutputPort : public Port
{
public:

    /// Default constructor.
    OutputPort();

    /**
     * Construct an output port.
     * @param name Port name.
     * @param type Port data type.
     */
    OutputPort(const QString &name, QVariant::Type type);

    /**
     * Returns value stored in this port.
     * @return
     */
    QVariant value() const override { return m_value; }

    /**
     * Assign value to this port.
     * @param value
     */
    void setValue(const QVariant &value);

    /**
     * Update the port.
     * This will trigger corresponding audio unit update that eventually
     * will assign the output port value.
     */
    void update() override;

    /**
     * Returns this output port index (within the list of all outputs of the audio unit).
     * @return
     */
    int index() const override final;
private:

    /// Value stored in this output port.
    QVariant m_value;
};

#endif // OUTPUTPORT_H

