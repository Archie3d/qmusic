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

#ifndef EXPRSYNTAXHIGHLIGHTER_H
#define EXPRSYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegExp>
#include <QVector>
#include <QTextCharFormat>

/**
 * @brief Exprtk expression syntax highlighter.
 */
class ExprSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    ExprSyntaxHighlighter(QTextDocument *pParent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:

    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> m_highlightingRules;
};

#endif // EXPRSYNTAXHIGHLIGHTER_H

