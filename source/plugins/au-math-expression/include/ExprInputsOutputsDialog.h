#ifndef EXPRINPUTSOUTPUTSDIALOG_H
#define EXPRINPUTSOUTPUTSDIALOG_H

#include <QDialog>

class QSpinBox;

/**
 * Dialog box asking for how many inputs and outputs the expression should have.
 */
class ExprInputsOutputsDialog : public QDialog
{
    Q_OBJECT

public:

    ExprInputsOutputsDialog(QWidget *pParent = nullptr);

    /**
     * Returns entered number of inputs.
     * @return
     */
    int numberOfInputs() const;

    /**
     * Returns entered number of outputs.
     * @return
     */
    int numberOfOutputs() const;

private:

    /// Number of inputs edit box
    QSpinBox *m_pNInputsSpinBox;

    /// Number of outputs edit box
    QSpinBox *m_pNOutputsSpinBox;
};

#endif // EXPRINPUTSOUTPUTSDIALOG_H

