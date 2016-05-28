/*
                          qmusic

    Copyright (C) 2015 Arthur Benilov,
    arthur.benilov@gmail.com

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.
*/

#include <QTextCharFormat>
#include "ExprSyntaxHighlighter.h"

const QColor cKeywordColor(100, 120, 200);
const QColor cConstColor(200, 60, 200);
const QColor cStringColor(Qt::darkGreen);
const QColor cCommentColor(80, 80, 80);
const QColor cReservedColor(220, 20, 20);
const QColor cFuncColor(60, 200, 200);

ExprSyntaxHighlighter::ExprSyntaxHighlighter(QTextDocument *pParent)
    : QSyntaxHighlighter(pParent)
{
    HighlightingRule rule;

    QTextCharFormat fmtKeyword;
    fmtKeyword.setForeground(cKeywordColor);
    fmtKeyword.setFontWeight(QFont::Bold);

    QStringList keywords;
    keywords << "if" << "else" << "while" << "and" << "or"
             << "null" << "var" << "switch" << "case" << "default";

    for (const QString &keyword : keywords) {
        rule.pattern = QRegExp(QString("\\b%1\\b").arg(keyword));
        rule.format = fmtKeyword;
        m_highlightingRules.append(rule);
    }

    QTextCharFormat fmtReserved;
    fmtReserved.setForeground(cReservedColor);

    QStringList reserved;
    reserved << "x" << "y" << "t";

    for (const QString &res : reserved) {
        rule.pattern = QRegExp(QString("\\b%1\\b").arg(res));
        rule.format = fmtReserved;
        m_highlightingRules.append(rule);
    }

    QTextCharFormat fmtConstants;
    fmtConstants.setForeground(cConstColor);
    fmtConstants.setFontWeight(QFont::Bold);

    QStringList constants;
    constants << "pi" << "sr";

    for (const QString &cnt : constants) {
        rule.pattern = QRegExp(QString("\\b%1\\b").arg(cnt));
        rule.format = fmtConstants;
        m_highlightingRules.append(rule);
    }

    QTextCharFormat fmtQuotation;
    fmtQuotation.setForeground(cStringColor);
    fmtQuotation.setFontItalic(true);
    rule.pattern = QRegExp("'.*'");
    rule.format = fmtQuotation;
    m_highlightingRules.append(rule);

    QTextCharFormat fmtFunction;
    fmtFunction.setForeground(cFuncColor);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = fmtFunction;
    m_highlightingRules.append(rule);

    QTextCharFormat fmtComment;
    fmtComment.setForeground(cCommentColor);
    rule.pattern = QRegExp("#[^\n]*");
    rule.format = fmtComment;
    m_highlightingRules.append(rule);
}

void ExprSyntaxHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : m_highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}
