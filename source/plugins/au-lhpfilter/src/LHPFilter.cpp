#include <QDebug>
#include <QtVariantPropertyManager>
#include <QtVariantProperty>
#include <qmath.h>
#include "Application.h"
#include "ISignalChain.h"
#include "../include/LHPFilter.h"


LHPFilter::LHPFilter(AudioUnitPlugin *pPlugin)
    : AudioUnit(pPlugin),
      m_filter()
{
    m_inputPtr = addInput("In", QVariant::Double);
    m_inputCutOffFreqPtr = addInput("f", QVariant::Double);

    m_outputPtr = addOutput("Out", QVariant::Double);

    createProperties();
}

LHPFilter::~LHPFilter()
{
}

void LHPFilter::processStart()
{
    m_filter.setSampleRate(44100.0);
    m_filter.reset();
}

void LHPFilter::processStop()
{
}

void LHPFilter::process()
{
    int type = m_pFilterType->value().toInt();
    m_filter.setType(type == 0 ? VAOnePoleFilter::Type_LP : VAOnePoleFilter::Type_HP);
    m_filter.setCutOffFrequency(m_inputCutOffFreqPtr->value().toDouble());
    m_outputPtr->setValue(m_filter.doFilter(m_inputPtr->value().toDouble()));
}

void LHPFilter::reset()
{
    m_filter.reset();
}

void LHPFilter::control(const QString &name, const QVariant &value)
{
}

void LHPFilter::createProperties()
{
    QtVariantProperty *pRoot = rootProperty();

    m_pFilterType = propertyManager()->addProperty(QtVariantPropertyManager::enumTypeId(), "Filter type");
    QVariantList list;
    list << "Low-pass" << "High-pass";
    m_pFilterType->setAttribute("enumNames", list);
    m_pFilterType->setValue(0);
    pRoot->addSubProperty(m_pFilterType);

}
