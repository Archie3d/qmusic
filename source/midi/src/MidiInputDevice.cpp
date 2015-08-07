#include <qglobal.h>
#include <QDebug>
#ifdef Q_OS_WIN
#   include <Windows.h>
#   include <mmsystem.h>
#endif
#include <QMap>
#include <QList>
#include "../include/MidiMessage.h"
#include "../include/MidiInputDevice.h"

const QMap<MMRESULT, QString> cErrToString = []() {
    QMap<MMRESULT, QString> map;
    map[MMSYSERR_NOERROR] = "No error";
    map[MMSYSERR_ALLOCATED] = "The specified resource is already allocated";
    map[MMSYSERR_BADDEVICEID] = "The specified device identifier is out of range";
    map[MMSYSERR_INVALFLAG] = "Invalid flag specified";
    map[MMSYSERR_INVALPARAM] = "Invalid parameter specified";
    map[MMSYSERR_NOMEM] = "The system is unable to allocate or lock memory";
    return map;
}();


// can be used for short, unsigned short, word, unsigned word (2-byte types)
#define BYTESWAP16(n) (((n&0xFF00)>>8)|((n&0x00FF)<<8))

// can be used for int or unsigned int or float (4-byte types)
#define BYTESWAP32(n) ((BYTESWAP16((n&0xFFFF0000)>>16))|((BYTESWAP16(n&0x0000FFFF))<<16))

// can be used for unsigned long long or double (8-byte types)
#define BYTESWAP64(n) ((BYTESWAP32((n&0xFFFFFFFF00000000)>>32))|((BYTESWAP32(n&0x00000000FFFFFFFF))<<32))

struct MidiInputDevicePrivate
{
    HMIDIIN handle; ///< System handle.
    bool deviceIsOpen;
    QList<IMidiInputListener*> listeners;
};

void CALLBACK midiInProc(HMIDIIN hMidiIn, UINT msg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
    Q_ASSERT(dwInstance);
    Q_UNUSED(hMidiIn);
    Q_UNUSED(dwParam1);
    Q_UNUSED(dwParam2);
    MidiInputDevice *pDev = reinterpret_cast<MidiInputDevice*>(dwInstance);

    switch(msg) {
    case MIM_OPEN:
        break;
    case MIM_CLOSE:
        break;
    case MIM_DATA:
        pDev->acceptMessage(BYTESWAP32(dwParam1) >> 8);
        break;
    default:
        break;
    }
}

MidiInputDevice::MidiInputDevice(int number)
    : MidiDevice(Type_Input, number)
{
    m = new MidiInputDevicePrivate;
    m->deviceIsOpen = false;
    setValid(validateDevice());
}

MidiInputDevice::~MidiInputDevice()
{
    close();
    delete m;
}

bool MidiInputDevice::open()
{
    if (isOpen()) {
        // Device is already open.
        return true;
    }

    MMRESULT r = midiInOpen(&m->handle,
                            number(),
                            reinterpret_cast<DWORD_PTR>(midiInProc),
                            reinterpret_cast<DWORD_PTR>(this),
                            CALLBACK_FUNCTION);

    if (r != MMSYSERR_NOERROR) {
        qDebug() << "MIDI In:" << cErrToString.value(r, "Unknown error");
        return false;
    }

    m->deviceIsOpen = true;
    return true;
}

bool MidiInputDevice::isOpen() const
{
    return m->deviceIsOpen;
}

bool MidiInputDevice::start()
{
    if (!isOpen()) {
        return false;
    }
    MMRESULT r = midiInStart(m->handle);
    return r != MMSYSERR_NOERROR;
}

bool MidiInputDevice::stop()
{
    if (!isOpen()) {
        return false;
    }
    MMRESULT r = midiInStop(m->handle);
    return r != MMSYSERR_NOERROR;
}


bool MidiInputDevice::close()
{
    if (!isOpen()) {
        return true;
    }

    MMRESULT r = midiInClose(m->handle);
    if (r != MMSYSERR_NOERROR) {
        return false;
    }

    return true;
}

void MidiInputDevice::addListener(IMidiInputListener *pListener)
{
    if (pListener != nullptr) {
        m->listeners.append(pListener);
    }
}

void MidiInputDevice::removeListener(IMidiInputListener *pListener)
{
    m->listeners.removeAll(pListener);
}

void MidiInputDevice::acceptMessage(unsigned int msg)
{
    // Message received.
    MidiMessage midiMessage(msg);
    notifyListeners(midiMessage);
}

bool MidiInputDevice::validateDevice()
{
    if (number() < 0 || number() >= getNumberOfInputDevices()) {
        // Device number is out of range.
        return false;
    }

    MIDIINCAPS devCaps;
    MMRESULT r = midiInGetDevCaps(number(),
                                  &devCaps,
                                  sizeof(MIDIINCAPS));

    if (r != MMSYSERR_NOERROR) {
        // Unable to get device capabilities
        return false;
    }

    //QString devName = QString::fromWCharArray(devCaps.szPname);
    QString devName = QString::fromLatin1(devCaps.szPname);
    setName(devName);
    setManufacturerId(devCaps.wMid);
    setProductId(devCaps.wPid);

    return true;
}

void MidiInputDevice::notifyListeners(const MidiMessage &msg)
{
    foreach (IMidiInputListener *pListener, m->listeners) {
        pListener->inputMidiMessage(msg);
    }
}
