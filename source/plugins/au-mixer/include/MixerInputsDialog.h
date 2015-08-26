#ifndef MIXERINPUTSDIALOG_H
#define MIXERINPUTSDIALOG_H

#include <QDialog>

class QSpinBox;

/**
 * Dialog box asking for how many inputs the mixer should have.
 */
class MixerInputsDialog : public QDialog
{
    Q_OBJECT

public:

    MixerInputsDialog(QWidget *pParent = nullptr);

    /**
     * Returns entered number of inputs.
     * @return
     */
    int numberOfInputs() const;

private:

    /// Number of inputs edit box
    QSpinBox *m_pNInputsSpinBox;
};

#endif // MIXERINPUTSDIALOG_H

