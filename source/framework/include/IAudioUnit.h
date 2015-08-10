#ifndef IAUDIOUNIT_H
#define IAUDIOUNIT_H

#include <QString>
#include <QVariant>
#include <QGraphicsItem>
#include "ISerializable.h"
#include "FrameworkApi.h"

/**
 * @brief Audio unit interface.
 *
 * This is an interface to an audio unit. An audio unit implements
 * a signle step of signal ptocessing. It takes whatever is currently
 * provided on its inputs and generates outputs.
 */
class QMUSIC_FRAMEWORK_API IAudioUnit : public ISerializable
{
public:

    enum Flag {
        Flag_NoFlags = 0,
        Flag_NoTitle = 1,
        Flag_NoFrame = 2
    };


    // Destructor.
    virtual ~IAudioUnit() {}

    /**
     * Prepare for the update.
     */
    virtual void prepareUpdate() = 0;

    /**
     * Perform processing on the current state.
     */
    virtual void update() = 0;

    /**
     * Start processing.
     */
    virtual void start() = 0;

    /**
     * Stop processing.
     */
    virtual void stop() = 0;

    /**
     * @brief Tells whether the unit has been started.
     * @return
     */
    virtual bool isStarted() const = 0;

    /**
     * @brief Reset the audio unit to its initial state.
     */
    virtual void reset() = 0;

    /**
     * @brief Create a custom graphics view for this item
     * @return
     */
    virtual QGraphicsItem* graphicsItem() = 0;

    /**
     * @brief Audio unit representation color.
     * @return
     */
    virtual QColor color() const = 0;

    /**
     * Returns audio unit flags.
     * @return
     */
    virtual int flags() const = 0;
};

#endif // IAUDIOUNIT_H

