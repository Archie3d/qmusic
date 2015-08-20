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

#ifndef ISIGNALCHAINSCENECONTAINER_H
#define ISIGNALCHAINSCENECONTAINER_H

#include "FrameworkApi.h"
#include "SignalChainScene.h"

class QMUSIC_FRAMEWORK_API ISignalChainSceneContainer
{
public:

    virtual void setSignalChainScene(SignalChainScene *pScene) = 0;
    virtual SignalChainScene* signalChainScene() const = 0;
    virtual ~ISignalChainSceneContainer();
};

#endif // ISIGNALCHAINSCENECONTAINER_H

