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

#include "ExprInputsOutputsDialog.h"
#include "MathExpression.h"
#include "MathExpressionPlugin.h"

MathExpressionPlugin::MathExpressionPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon MathExpressionPlugin::icon() const
{
    return QIcon(":/au-math-expression/icon.png");
}

AudioUnit* MathExpressionPlugin::createInstance()
{
    return new MathExpression(this);
}

AudioUnit* MathExpressionPlugin::createInstanceInteractive()
{
    MathExpression *pUnit = nullptr;
    ExprInputsOutputsDialog dlg;
    if (dlg.exec() == QDialog::Accepted) {
        pUnit = new MathExpression(this);
        pUnit->createPorts(dlg.numberOfInputs(),
                           dlg.numberOfOutputs());
    }
    return pUnit;
}
