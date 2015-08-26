#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QPushButton>
#include "Mixer.h"
#include "MixerInputsDialog.h"

MixerInputsDialog::MixerInputsDialog(QWidget *pParent)
    : QDialog(pParent)
{
    setWindowTitle(tr("Mixer inputs"));
    setWindowIcon(QIcon(":/au-mixer/icon.png"));

    // Create layout
    QVBoxLayout *pLayout = new QVBoxLayout();

    QFormLayout *pFormLayout = new QFormLayout();
    m_pNInputsSpinBox = new QSpinBox();
    m_pNInputsSpinBox->setRange(Mixer::cMinNumberOfInputs, Mixer::cMaxNumberOfInputs);
    m_pNInputsSpinBox->setValue(Mixer::cMinNumberOfInputs);
    pFormLayout->addRow(tr("Number of inputs"), m_pNInputsSpinBox);
    pLayout->addLayout(pFormLayout);

    QHBoxLayout *pButtonsLayout = new QHBoxLayout();
    pButtonsLayout->addStretch();

    QPushButton *pOkButton = new QPushButton(tr("OK"));
    pOkButton->setDefault(true);
    connect(pOkButton, SIGNAL(clicked()), this, SLOT(accept()));
    pButtonsLayout->addWidget(pOkButton);

    QPushButton *pCancelButton = new QPushButton(tr("Cancel"));
    connect(pCancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    pButtonsLayout->addWidget(pCancelButton);

    pLayout->addLayout(pButtonsLayout);

    setLayout(pLayout);
}

int MixerInputsDialog::numberOfInputs() const
{
    return m_pNInputsSpinBox->value();
}
