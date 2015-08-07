#ifndef MIDIDEVICE_H
#define MIDIDEVICE_H

#include <QList>
#include <QString>

#include "MidiApi.h"

/**
 * @brief Abstract class defining a MIDI device.
 */
class QMUSIC_MIDI_API MidiDevice
{
public:

    /**
     * Device type, inout or output.
     */
    enum Type {
        Type_Input,
        Type_Output
    };

    /**
     * Device description structure.
     */
    struct Description {
        Type type;
        int number;
        QString name;
    };

    MidiDevice(Type type, int number = 0);
    virtual ~MidiDevice() {}

    Type type() const { return m_type; }
    int number() const { return m_number; }
    void setNumber(int n) { m_number = n; }
    int channel() const { return m_channel; }
    void setChannel(int ch) { m_channel = ch; }
    QString name() const { return m_name; }
    quint16 manufacturerId() const { return m_manufacturerId; }
    quint16 productId() const { return m_productId; }

    /**
     * @brief Tells whether this device is valid and can be used.
     * @return true if device is valid, false elsewhere.
     */
    bool isValid() const { return m_valid; }

    virtual bool open() = 0;
    virtual bool close() = 0;
    virtual bool isOpen() const = 0;

    // Static API methods
    static int getNumberOfInputDevices();
    static int getNumberOfOutputDevices();
    static QList<Description> enumerateDevices();
    static QList<Description> enumerateInputDevices();
    static QList<Description> enumerateOutputDevices();

protected:

    void setValid(bool v) { m_valid = v; }
    void setName(const QString &n) { m_name = n; }
    void setManufacturerId(quint16 id) { m_manufacturerId = id; }
    void setProductId(quint16 id) { m_productId = id; }

private:

    bool m_valid;
    int m_number;   ///< Device number.
    int m_channel;  ///< Midi channel.
    Type m_type;    ///< Device type.
    QString m_name; ///< Device name.
    quint16 m_manufacturerId;   ///< Device manufacturer ID.
    quint16 m_productId;        ///< Device product ID.
};

#endif // MIDIDEVICE_H

