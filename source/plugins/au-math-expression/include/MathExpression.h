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

#include <vector>
#include <QVector>
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

    void createPorts(int nInputs, int nOutputs);

    QColor color() const { return QColor(64, 64, 64); }

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

    QVector<InputPort*> m_inputs;
    QVector<OutputPort*> m_outputs;

    // exprtk
    SymbolTable m_symbolTable;
    Expression m_expression;
    float m_sampleRate; // Sample rate.
    float m_t;  // Time.
    std::vector<float> m_xVector;
    std::vector<float> m_yVector;
    long long m_timeStep;
    bool m_compiledOk;

    QString m_script;
};

#endif // AU_MATH_EXPRESSION_H

