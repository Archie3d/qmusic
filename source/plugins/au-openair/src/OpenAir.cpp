#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "FIRFilter.h"
#include "OpenAirPlugin.h"
#include "OpenAir.h"


OpenAir::OpenAir(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin)
{
    m_pInput = addInput("in", QVariant::Double);
    m_pOutput = addOutput("out", QVariant::Double);

    createProperties();

    m_pFIRFilter = nullptr;
}

OpenAir::~OpenAir()
{
    delete m_pFIRFilter;
}

void OpenAir::serialize(QVariantMap &data, SerializationContext *pContext) const
{
    Q_ASSERT(pContext != nullptr);
    data["environment"] = m_pPropEnvironment->value();
    AudioUnit::serialize(data, pContext);
}

void OpenAir::deserialize(const QVariantMap &data, SerializationContext *pContext)
{
    Q_ASSERT(pContext != nullptr);
    m_pPropEnvironment->setValue(data["environment"]);
    AudioUnit::deserialize(data, pContext);
}

void OpenAir::processStart()
{
    createFIR();
}

void OpenAir::processStop()
{
}

void OpenAir::process()
{
    m_pOutput->setValue(m_pFIRFilter->process(m_pInput->value().toDouble()));
}

void OpenAir::reset()
{
}

void OpenAir::createProperties()
{
    OpenAirPlugin *pPlugin = dynamic_cast<OpenAirPlugin*>(plugin());
    QtVariantProperty *pRoot = rootProperty();

    m_pPropEnvironment = propertyManager()->addProperty(QtVariantPropertyManager::enumTypeId(), "Filter type");
    QVariantList list;
    foreach (const QString &env, pPlugin->environments()) {
        list.append(env);
    }

    m_pPropEnvironment->setAttribute("enumNames", list);
    m_pPropEnvironment->setValue(0);
    pRoot->addSubProperty(m_pPropEnvironment);
}

void OpenAir::createFIR()
{
    delete m_pFIRFilter;
    OpenAirPlugin *pPlugin = dynamic_cast<OpenAirPlugin*>(plugin());
    QVector<float> ir = pPlugin->impulseResponse(m_pPropEnvironment->valueText());

    m_pFIRFilter = new FIRFilter(ir);
}
