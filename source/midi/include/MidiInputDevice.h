#ifndef MIDIINPUTDEVICE_H
#define MIDIINPUTDEVICE_H

#include "MidiDevice.h"
#include "MidiMessage.h"
#include "MidiApi.h"

struct MidiInputDevicePrivate;

class IMidiInputListener
{
public:

    virtual void inputMidiMessage(const MidiMessage &msg) = 0;
    virtual ~IMidiInputListener() {}

};

class QMUSIC_MIDI_API MidiInputDevice : public MidiDevice
{
public:

    /**
     * @brief Construct MIDI input device.
     * @param number Device number.
     */
    MidiInputDevice(int number);
    ~MidiInputDevice();

    bool open();
    bool close();
    bool isOpen() const;

    bool start();
    bool stop();

    void addListener(IMidiInputListener *pListener);
    void removeListener(IMidiInputListener *pListener);

    void acceptMessage(unsigned int msg);

private:

    /**
     * Fetch device description and update valid flag.
     * @return true if device has been validated.
     */
    bool validateDevice();

    void notifyListeners(const MidiMessage &msg);

    MidiInputDevicePrivate *m;
};

#endif // MIDIINPUTDEVICE_H

