#ifndef ISIGNALCHAIN_H
#define ISIGNALCHAIN_H

#include "ISerializable.h"
#include "FrameworkApi.h"

class IAudioUnit;

/**
 * @brief Interface to a signal chain.
 * Signal chain is an agglomeration of audio units connected together.
 * The chain keep track of the global time.
 */
class QMUSIC_FRAMEWORK_API ISignalChain : public ISerializable
{
public:

    /**
     * @brief Start the audio chain.
     * This will start all audio units.
     */
    virtual void start() = 0;

    /**
     * @brief Stop the audio chain.
     * This will stop all audio units.
     */
    virtual void stop() = 0;

    /**
     * @brief Tells whether the audio chain processing has been started.
     * @return true if chain processing has been started.
     */
    virtual bool isStarted() const = 0;

    /**
     * @brief Reset the audio chain.
     * This will call reset of all audio units.
     * The global time will be also reset to zero.
     */
    virtual void reset() = 0;

    /**
     * @brief Returns time step used to advance time
     * @return
     */
    virtual double timeStep() const = 0;

    /**
     * @brief Set synth time step
     * @param dt Time step in seconds.
     */
    virtual void setTimeStep(double dt) = 0;

    /**
     * @brief Add an audio unit to this chain.
     * @param pAudioUnit
     */
    virtual void addAudioUnit(IAudioUnit *pAudioUnit) = 0;

    /**
     * @brief Remove the audio unit from this chain.
     * @param pAudioUnit
     */
    virtual void removeAudioUnit(IAudioUnit *pAudioUnit) = 0;

    /**
     * @brief Prepare all audio units for update.
     */
    virtual void prepareUpdate() = 0;

    /// Destructor.
    virtual ~ISignalChain() {}
};

#endif // ISIGNALCHAIN_H

