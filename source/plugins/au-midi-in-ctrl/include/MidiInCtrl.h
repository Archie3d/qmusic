#ifndef AU_MIDIIN_H
#define AU_MIDIIN_H

#include <QObject>
#include "AudioUnit.h"
#include "MidiMessage.h"
#include "MidiInputDevice.h"

class QtVariantProperty;
class MidiInputDevice;

class MidiInCtrl : public QObject,
                   public AudioUnit,
                   public IMidiInputListener
{
    Q_OBJECT
public:

    MidiInCtrl(AudioUnitPlugin *pPlugin);
    ~MidiInCtrl();

    QColor color() const override;

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const override;
    void deserialize(const QVariantMap &data, SerializationContext *pContext) override;

public slots:
    void updateView();

signals:

    void triggerUpdateView();

protected:

    void processStart();
    void processStop();
    void process();
    void reset();

    QGraphicsItem* graphicsItem();

    // IMidiInputListener
    void inputMidiMessage(const MidiMessage &msg);

private:

    void createProperties();

    MidiMessage::Controller m_controller;
    int m_controllerValue;
    double m_controllerValueNormalized;
    QString m_controllerName;

    OutputPort *m_pOutputValue;

    QGraphicsSimpleTextItem *m_pValueItem;

    QtVariantProperty *m_pPropController;
    QtVariantProperty *m_pPropDefaultValue;
};

#endif // AU_MIDIIN_H

