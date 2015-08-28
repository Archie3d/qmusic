#ifndef EXPREDITORDIALOG_H
#define EXPREDITORDIALOG_H

#include <QDialog>

class QTextEdit;
class ExprSyntaxHighlighter;

/**
 * @brief Dialog box containing expr editor.
 */
class ExprEditorDialog : public QDialog
{
    Q_OBJECT
public:

    ExprEditorDialog(QWidget *pParent = nullptr);

    QString script() const;
    void setScript(const QString &s);

private:

    QTextEdit *m_pTextEdit;
    ExprSyntaxHighlighter *m_pExprSyntaxHighlighter;
};

#endif // EXPREDITORDIALOG_H

