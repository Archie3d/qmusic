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

#include "Mixer.h"
#include "MixerInputsDialog.h"
#include "MixerPlugin.h"

MixerPlugin::MixerPlugin(QObject *pParent)
    : AudioUnitPlugin(pParent)
{
}

QIcon MixerPlugin::icon() const
{
    return QIcon(":/au-mixer/icon.png");
}

AudioUnit* MixerPlugin::createInstance()
{
    return new Mixer(this);
}

AudioUnit* MixerPlugin::createInstanceInteractive()
{
    Mixer *pMixer = nullptr;
    MixerInputsDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        pMixer = new Mixer(this);
        pMixer->createInputs(dialog.numberOfInputs());
    }

    return pMixer;
}
