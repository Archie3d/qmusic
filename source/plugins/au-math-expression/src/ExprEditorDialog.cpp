#include <QVBoxLayout>
#include <QTextEdit>
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

    QVBoxLayout *pButtonsLayout = new QVBoxLayout();
    pButtonsLayout->setMargin(6);

    QDialogButtonBox *pButtonBox = new QDialogButtonBox();
    QPushButton *pOkButton = pButtonBox->addButton(QDialogButtonBox::Ok);
    QPushButton *pCancelButton = pButtonBox->addButton(QDialogButtonBox::Cancel);

    connect(pOkButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(pCancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    pButtonsLayout->addWidget(pButtonBox);

    pLayout->addWidget(m_pTextEdit);
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
