/*$Id: ApConnectStateObserver.h 303 2008-12-14 14:10:36Z ezhenzh $*/
/*
 * PktPing - Pocket Ping for S60 3rd
 * Copyright (C) 2008 Reggie Zhang <reggie.zhang@mobiledev.org.cn>
 * Licensed under the terms of The GNU Lesser General Public License (LGPLv3):
 * http://www.opensource.org/licenses/lgpl-3.0.html
 * 
 */

#ifndef APCONNECTSTATEOBSERVER_H
#define APCONNECTSTATEOBSERVER_H

// INCLUDES

#include <e32std.h>

// CLASS DECLARATION

enum TAPConnectEngineStates // Engine states
{
    EApConnectStateNotConnected,
    EApConnectStateSelecting,
    EApConnectStateConnecting,
    EApConnectStateConnected,
    EApConnectStateDisconnecting,
    EApConnectStateUnknown
};

/**
* MAPConnectStateObserver  State Observer interface
*/
class MApConnectStateObserver 
    {
public:

    /*
     * ChangeStateL()
     *  
     * Notify State change
     * 
     * Params:
     * TInt aState   State value
     *
     */
    virtual void ChangeStateL(TInt aState) = 0;
    };

#endif // APCONNECTSTATEOBSERVER_H
