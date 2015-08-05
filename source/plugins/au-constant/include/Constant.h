#ifndef AUDIOSINK_H
#define AUDIOSINK_H

#include "AudioUnit.h"

class QtVariantProperty;
class QGraphicsSimpleTextItem;

class Constant : public AudioUnit
{
public:

    Constant(AudioUnitPlugin *pPlugin);
    ~Constant();

protected:

    void processStart();
    void processStop();
    void process();
    void reset();
    void control(const QString &name, const QVariant &value);
    QGraphicsItem* graphicsItem();
    QColor color() const override;
    int flags() const;

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);

private:

    void createProperties();

    QGraphicsSimpleTextItem *m_pValueItem;

    OutputPort *m_pOutput;
    QtVariantProperty *m_pPropConstant;
};

#endif // AUDIOSINK_H

