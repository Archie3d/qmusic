#include <QVBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include "ExprSyntaxHighlighter.h"
#include "ExprEditorDialog.h"

ExprEditorDialog::ExprEditorDialog(QWidget *pParent)
    : QDialog(pParent)
{
    setWindowTitle(tr("Expression"));
    setWindowIcon(QIcon(":/au-math-expression/icon.png"));

    QVBoxLayout *pLayout = new QVBoxLayout();
    pLayout->setMargin(0);

    QFont font;
    font.setFamily("Consolas");
    font.setFixedPitch(true);
    font.setPointSize(10);

    m_pTextEdit = new QTextEdit();
    m_pTextEdit->setFont(font);

    m_pExprSyntaxHighlighter = new ExprSyntaxHighlighter(m_pTextEdit->document());

    QVBoxLayout *pWarningLayout = new QVBoxLayout();
    pWarningLayout->setMargin(6);
    QLabel *pWarningLabel = new QLabel(tr("Watch out: errors in the script may cause the entire application to crash."));

    pWarningLayout->addWidget(pWarningLabel);

    QVBoxLayout *pButtonsLayout = new QVBoxLayout();
    pButtonsLayout->setMargin(6);

    QDialogButtonBox *pButtonBox = new QDialogButtonBox();
    QPushButton *pOkButton = pButtonBox->addButton(QDialogButtonBox::Ok);
    QPushButton *pCancelButton = pButtonBox->addButton(QDialogButtonBox::Cancel);

    connect(pOkButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(pCancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    pButtonsLayout->addWidget(pButtonBox);

    pLayout->addWidget(m_pTextEdit);
    pLayout->addLayout(pWarningLayout);
    pLayout->addLayout(pButtonsLayout);

    setLayout(pLayout);

    setModal(true);
}

QString ExprEditorDialog::script() const
{
    return m_pTextEdit->toPlainText();
}

void ExprEditorDialog::setScript(const QString &s)
{
    m_pTextEdit->setPlainText(s);
}
