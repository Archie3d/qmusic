#include <QVBoxLayout>
#include <QFormLayout>
#include <QSpinBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include "ExprInputsOutputsDialog.h"

ExprInputsOutputsDialog::ExprInputsOutputsDialog(QWidget *pParent)
    : QDialog(pParent)
{
    setWindowTitle(tr("Expression inputs/outputs"));
    setWindowIcon(QIcon(":/au-math-expression/icon.png"));

    // Create layout
    QVBoxLayout *pLayout = new QVBoxLayout();

    QFormLayout *pFormLayout = new QFormLayout();

    m_pNInputsSpinBox = new QSpinBox();
    m_pNInputsSpinBox->setRange(0, 8);
    m_pNInputsSpinBox->setValue(1);
    pFormLayout->addRow(tr("Number of inputs"), m_pNInputsSpinBox);

    m_pNOutputsSpinBox = new QSpinBox();
    m_pNOutputsSpinBox->setRange(1, 8);
    m_pNOutputsSpinBox->setValue(1);
    pFormLayout->addRow(tr("Number of outputs"), m_pNOutputsSpinBox);


    pLayout->addLayout(pFormLayout);

    QDialogButtonBox *pButtonBox = new QDialogButtonBox();
    QPushButton *pOkButton = pButtonBox->addButton(QDialogButtonBox::Ok);
    connect(pOkButton, SIGNAL(clicked()), this, SLOT(accept()));

    QPushButton *pCancelButton = pButtonBox->addButton(QDialogButtonBox::Cancel);
    connect(pCancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    pLayout->addWidget(pButtonBox);

    setLayout(pLayout);
}

int ExprInputsOutputsDialog::numberOfInputs() const
{
    return m_pNInputsSpinBox->value();
}

int ExprInputsOutputsDialog::numberOfOutputs() const
{
    return m_pNOutputsSpinBox->value();
}
