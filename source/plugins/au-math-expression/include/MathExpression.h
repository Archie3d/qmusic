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

#ifndef AU_MATH_EXPRESSION_H
#define AU_MATH_EXPRESSION_H

#include "exprtk.hpp"
#include "AudioUnit.h"

class QtVariantProperty;

class MathExpression : public AudioUnit
{
public:

    typedef exprtk::symbol_table<float> SymbolTable;
    typedef exprtk::expression<float> Expression;
    typedef exprtk::parser<float> Parser;

    MathExpression(AudioUnitPlugin *pPlugin);
    ~MathExpression();

    QColor color() const { return QColor(250, 250, 250); }

    QGraphicsItem *graphicsItem();

    // ISerializable interface
    void serialize(QVariantMap &data, SerializationContext *pContext) const;
    void deserialize(const QVariantMap &data, SerializationContext *pContext);

protected:

    void processStart();
    void processStop();
    void process();
    void reset();

private slots:

    void showScriptEditor();

private:

    static QString removeScriptComments(const QString &src);

    InputPort *m_pInput;
    OutputPort *m_pOutput;

    // exprtk
    SymbolTable m_symbolTable;
    Expression m_expression;
    float m_sampleRate; // Sample rate.
    float m_t;  // Time.
    float m_x;  // Input argument.
    long long m_timeStep;
    bool m_compiledOk;

    QString m_script;
};

#endif // AU_MATH_EXPRESSION_H

